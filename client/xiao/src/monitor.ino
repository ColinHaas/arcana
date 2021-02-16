#define MARK_EXCESS_MICROS 20 // recommended for the cheap VS1838 modules
#include <ArduinoJson.h>
#include <IRremote.h>
#include <SparkFunBME280.h>
#include <Wire.h>

const uint8_t IR_DATA_PIN = 1;
const uint8_t FAN_PWM_PIN = 2;
const uint8_t RELAY_ENABLE_PIN = 3;

const uint32_t BAUD_RATE = 115200;
const unsigned int BUFFER_SIZE = 256;

const int DEFAULT_LOW = 40;
const int DEFAULT_HIGH = 50;
const int DEFAULT_CRITICAL = 60;
const int DEFAULT_FAN = 100;
const int DEFAULT_INTERVAL = 5000;

BME280 sensor;
bool sensing = false;
IRrecv receiver(IR_DATA_PIN, LED_BUILTIN);
decode_results results;

#define UART SerialUSB

void status();
void process(char *message);
void error(char *tag, char *info);

struct
{
    int low;
    int high;
    int critical;
    int fan;
    int interval;

    void reset()
    {
        low = DEFAULT_LOW;
        high = DEFAULT_HIGH;
        critical = DEFAULT_CRITICAL;
        fan = DEFAULT_FAN;
        interval = DEFAULT_INTERVAL;
    }

} settings;

void status()
{
    // populate status message parameters
    StaticJsonDocument<256> json;
    JsonObject data = json.createNestedObject("status");
    data["temperature"] = sensor.readTempC();
    data["pressure"] = sensor.readFloatPressure();
    data["humidity"] = sensor.readFloatHumidity();
    JsonObject settings_data = data.createNestedObject("settings");
    settings_data["low"] = settings.low;
    settings_data["high"] = settings.high;
    settings_data["critical"] = settings.critical;
    settings_data["fan"] = settings.fan;
    settings_data["interval"] = settings.interval;

    // send message to controller
    serializeJson(json, UART);
    UART.println();
}

void process(char *message)
{
    // parse out key-value pair from message
    char key = message[0];
    int value = atoi(message + 1);

    // execute command corresponding to key
    switch (key)
    {
    case 'L': settings.low = constrain(value, 1, (settings.high - 1)); break;
    case 'H': settings.high = constrain(value, (settings.low + 1), (settings.critical - 1)); break;
    case 'C': settings.critical = constrain(value, (settings.high + 1), 80); break;
    case 'F': settings.fan = constrain(value, 0, 255); break;
    case 'I': settings.interval = constrain(value, 0, 60000); break;
    case 'R': settings.reset(); break;
    case 'S': status(); break;
    }
}

void error(char *tag, char *info)
{
    // populate error message parameters
    StaticJsonDocument<256> json;
    JsonObject data = json.createNestedObject("error");
    data["tag"] = tag;
    data["info"] = info;

    // send error message to controller
    serializeJson(json, UART);
    UART.println();
}

void setup()
{
    // open serial communication with controller
    UART.begin(BAUD_RATE);

    // configure pin funcionality
    pinMode(FAN_PWM_PIN, OUTPUT);
    pinMode(RELAY_ENABLE_PIN, OUTPUT);
    digitalWrite(RELAY_ENABLE_PIN, LOW);

    // configure settings with default values
    settings.reset();

    // enable reception of infrared data
    receiver.enableIRIn();

    // initialize atmospheric sensor interface
    Wire.begin();
    sensor.setI2CAddress(0x76);
    sensing = sensor.beginI2C();
}

void loop()
{
    ////////////////////////////////////////////////////////////////////////////////
    // CONTROLLER INTERFACE
    ////////////////////////////////////////////////////////////////////////////////

    // check for serial commands from controller
    if (UART.available() > 0)
    {
        static char buffer[BUFFER_SIZE];
        static unsigned int position = 0;
        static bool overflow = false;

        // read serial data from controller
        char byte = UART.read();

        // check for serial data line break
        if (byte == '\n')
        {
            // only process messages with valid length
            if (!overflow)
            {
                // add null terminator and process message
                buffer[position] = '\0';
                process(buffer);
            }

            // reset buffer position and overflow flag
            overflow = false;
            position = 0;
        }
        else
        {
            // determine if buffer length has been exceeded
            overflow = (position > (BUFFER_SIZE - 2));

            // append serial data to buffer if no overflow
            if (!overflow)
                buffer[position] = byte;

            // increment buffer position
            position += 1;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // INFRARED RECEIVER
    ////////////////////////////////////////////////////////////////////////////////

    // check for data from infrared receiver
    if (receiver.decode())
    {
        // populate infrared data message parameters
        StaticJsonDocument<256> json;
        JsonObject data = json.createNestedObject("infrared");
        data["protocol"] = receiver.getProtocolString();
        data["address"] = receiver.decodedIRData.address;
        data["command"] = receiver.decodedIRData.command;
        data["value"] = receiver.results.value;
        data["flags"] = receiver.decodedIRData.flags;

        // send message to controller
        serializeJson(json, UART);
        UART.println();

        // enable receiving of the next value
        receiver.resume();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // THERMAL MANAGEMENT
    ////////////////////////////////////////////////////////////////////////////////

    // read data from sensor
    float temperature = sensor.readTempC();
    float pressure = sensor.readFloatPressure();
    float humidity = sensor.readFloatHumidity();

    // enable or disable relay based on thermal conditions
    if (temperature < settings.high)
        digitalWrite(RELAY_ENABLE_PIN, HIGH);
    if (temperature > settings.critical)
        digitalWrite(RELAY_ENABLE_PIN, LOW);

    // determine fan speed based on temperature
    uint8_t fan = 0;
    if (temperature > settings.high)
    {
        fan = 255;
    }
    else if (temperature > settings.low)
    {
        float temperature_delta = temperature - settings.low;
        float temperature_range = settings.high - settings.low;
        float speed_range = 255 - settings.fan;
        float percent = (temperature_range > 0) ? (temperature_delta / temperature_range) : 1.0;
        fan = uint8_t(settings.fan + speed_range * percent);
    }

    // update current fan speed
    analogWrite(FAN_PWM_PIN, fan);

    // send status messages at regular intervals
    static unsigned long reported;
    if (millis() - reported > settings.interval)
    {
        reported = millis();
        status();
    }
}
