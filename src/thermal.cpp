#define SYSLOG_DEBUG
#include <psyslog.h>

#include "thermal.h"
#include "config.h"

Adafruit_BME280 Thermal::bme = Adafruit_BME280();
uint32_t Thermal::updated = 0;
uint32_t Thermal::reported = 0;
float Thermal::temperature = 0;
float Thermal::pressure = 0;
float Thermal::humidity = 0;

bool Thermal::setup()
{
    if (!bme.begin())
    {
        LOGC("error");
        return false;
    }

    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::FILTER_X16,
                    Adafruit_BME280::STANDBY_MS_0_5);
    return true;
}

void Thermal::update()
{
    if ((millis() - updated) >= THERMAL_UPDATE_INTERVAL)
    {
        temperature = bme.readTemperature();
        pressure = bme.readPressure();
        humidity = bme.readHumidity();

        updated = millis();
    }

    if ((millis() - reported) > THERMAL_REPORT_INTERVAL)
    {
        LOGD("temperature=%f, pressure=%f, humidity=%f", temperature, pressure, humidity);
        reported = millis();
    }
}
