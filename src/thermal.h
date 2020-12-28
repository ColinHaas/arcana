#ifndef __THERMAL_H__
#define __THERMAL_H__

#include <Particle.h>
#include <Adafruit_BME280.h>

class Thermal
{
public:
    static bool setup();
    static void update();

private:
    static Adafruit_BME280 bme;
    static uint32_t updated;
    static uint32_t reported;
    static float temperature;
    static float pressure;
    static float humidity;
};

#endif
