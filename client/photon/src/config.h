#ifndef __CONFIG_H___
#define __CONFIG_H___

#include <Particle.h>

// Network

static const uint8_t SERVER_IP_ADDRESS[4] = {192, 168, 10, 113};
static const uint16_t MQTT_DEFAULT_PORT = 1883;
static const clock_t MQTT_TOPIC_LENGTH_MAX = 100;
static const clock_t CLOUD_CONNECT_TIMEOUT = 30000;
static const clock_t CONNECT_RETRY_INTERVAL = 60000;
static const clock_t TIME_SYNC_INTERVAL = (24 * 60 * 60 * 1000);

static const clock_t REPORT_INTERVAL_HEARTBEAT = 5000;
static const clock_t REPORT_INTERVAL_PROXIMITY = 250;
static const clock_t REPORT_INTERVAL_TEMPERATURE = 10000;
static const clock_t REPORT_INTERVAL_LIGHTING = 5000;

// Audio

static const pin_t PIEZO_PIN = DAC;
static const uint16_t AUDIO_BUFFER_SIZE = 128;
static const uint16_t AUDIO_SAMPLING_FREQUENCY = 22050;
static const uint16_t AUDIO_TEST_FREQUENCY = 2000;
static const uint16_t AUDIO_TEST_TIME = 500;

// Status

static const pin_t BUILTIN_LED_PIN = D7;
static const uint8_t STATUS_LED_BRIGHTNESS = 10;

static const uint32_t RGB_STATUS_THERMAL = 0xFF0000; // red
static const uint32_t RGB_STATUS_MOTION = 0xFF8700;  // orange
static const uint32_t RGB_STATUS_AUDIO = 0xFFF700;   // yellow
static const uint32_t RGB_STATUS_DISPLAY = 0x18FF00; // green
static const uint32_t RGB_STATUS_NETWORK = 0x00EAFF; // cyan
static const uint32_t RGB_STATUS_TOUCH = 0x0000FF;   // blue
static const uint32_t RGB_STATUS_REMOTE = 0xD600FF;  // purple

// Display

static const pin_t PIXEL_PIN = D2;
static const clock_t UPDATE_INTERVAL_DISPLAY = 15;
static const uint16_t PIXEL_COUNT = 241;
static const uint8_t TSL2561_I2C_ADDRESS = 0x39;

// Touch

static const pin_t TOUCH_IRQ_PIN = D5;
static const clock_t UPDATE_INTERVAL_TOUCH = 10;
static const uint8_t PROXIMITY_ELECTRODE_COUNT = 8;
static const uint8_t PROXIMITY_ELECTRODE_PIN_OFFSET = 2;
static const uint8_t MPR121_I2C_ADDRESS = 0x5A;

// Thermal

static const clock_t UPDATE_INTERVAL_THERMAL = 1000;
static const uint8_t BME280_I2C_ADDRESS = 0x76;
static const pin_t FAN_CONTROL_PIN = A4;
static const uint8_t FAN_SPEED_MIN = 100;
static const uint8_t FAN_SPEED_MAX = 255;
static const float FAN_ENABLE_TEMP = 35.0;
static const float FAN_MAX_TEMP = 45.0;
static const clock_t THERMAL_ALARM_TIME = 30000;

// Motion

static const pin_t RADAR_SIGNAL_PIN = A2;

// Remote

static const pin_t IR_DATA_PIN = D6;

// Utility

template <typename Base, typename T> static inline bool instanceof (const T *)
{
    return std::is_base_of<Base, T>::value;
}

static inline bool wired(uint8_t address)
{
    Wire.begin();
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
    return (error == 0);
}

#endif
