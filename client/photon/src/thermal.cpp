#include <Adafruit_BME280.h>

#include "config.h"
#include "display.h"
#include "network.h"
#include "thermal.h"

bool Thermal::sensing = false;
clock_t Thermal::alarm = 0;

Adafruit_BME280 bme = Adafruit_BME280();

void Thermal::setup()
{
    pinMode(FAN_CONTROL_PIN, OUTPUT);

    if (wired(BME280_I2C_ADDRESS))
    {
        if (bme.begin())
        {
            bme.setSampling(Adafruit_BME280::MODE_NORMAL, Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::FILTER_X16, Adafruit_BME280::STANDBY_MS_0_5);

            sensing = true;
        }
    }

    if (!sensing)
        Serial.println("<ERROR> BME280 Setup");
}

void Thermal::update()
{
    static unsigned long updated;
    if ((millis() - updated) >= UPDATE_INTERVAL_THERMAL)
    {
        updated = millis();

        if (sensing)
        {
            // read temperature and report to server
            float temperature = bme.readTemperature();
            Network::temperature(temperature);

            // set fan speed based on temperature
            uint8_t speed = 0;
            if (temperature >= FAN_ENABLE_TEMP)
            {
                if (temperature >= FAN_MAX_TEMP)
                {
                    speed = 255;

                    // disable display if thermal alarm time exceeded
                    if (alarm > 0)
                    {
                        if (((millis() - alarm) > THERMAL_ALARM_TIME))
                        {
                            Display::enabled = false;
                            Serial.println("<THERMAL_SHUTDOWN>");
                        }
                    }
                    else
                    {
                        alarm = millis();
                        Serial.println("<TEMP_ALARM>");
                    }
                }
                else
                {
                    // reset over-temperature alarm
                    if (alarm != 0)
                    {
                        Display::enabled = true;
                        alarm = 0;
                        Serial.println("<TEMP_RESET>");
                    }

                    // calculate fan speed based on internal temperature
                    float percent = (temperature - FAN_ENABLE_TEMP) / (FAN_MAX_TEMP - FAN_ENABLE_TEMP);
                    float range = (FAN_SPEED_MAX - FAN_SPEED_MIN);
                    speed = uint8_t(FAN_SPEED_MIN + range * percent);
                }
            }

            analogWrite(FAN_CONTROL_PIN, speed);
        }
    }
}
