#ifndef __CONFIG_H___
#define __CONFIG_H___

#include <Particle.h>

// network
static const char *SERVER_URL = "nexi.local";
static const unsigned long TIMEOUT_WIFI_READY = 15000;
static const unsigned long TIMEOUT_TIME_SYNC = 10000;

// audio
static const uint8_t PIEZO_PIN = DAC;
static const uint16_t AUDIO_BUFFER_SIZE = 128;
static const uint16_t AUDIO_SAMPLING_FREQUENCY = 22050; // Hz
static const uint16_t AUDIO_TEST_FREQUENCY = 1000;      // Hz
static const uint16_t AUDIO_TEST_TIME = 1500;           // ms

// display
static const uint8_t PIXEL_PIN = D2;
static const uint8_t PIXEL_COUNT = 241;
static const uint8_t PIXEL_UPDATE_INTERVAL = 15;      // ms
static const uint32_t DISPLAY_REPORT_INTERVAL = 5000; // ms

// touch
static const uint8_t TOUCH_IRQ_PIN = D5;
static const uint8_t PROXIMITY_ELECTRODE_COUNT = 8;
static const uint8_t PROXIMITY_ELECTRODE_PIN_OFFSET = 2;

// thermal
static const float SEA_LEVEL_PRESSSURE = 1013.25;     // hPa
static const uint32_t THERMAL_UPDATE_INTERVAL = 500;  // ms
static const uint32_t THERMAL_REPORT_INTERVAL = 5000; // ms

#endif
