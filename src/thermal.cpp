#include <Adafruit_BME280.h>

#include "thermal.h"
#include "config.h"
#include "network.h"

bool Thermal::sensing = false;

Adafruit_BME280 bme = Adafruit_BME280();

void Thermal::setup()
{
    if (wired(BME280_I2C_ADDRESS))
    {
        if (bme.begin())
        {
            bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::FILTER_X16,
                            Adafruit_BME280::STANDBY_MS_0_5);

            sensing = true;
        }
    }

    if (!sensing)
        Serial.printlnf("<ERROR> BME280");
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
        }
    }
}
