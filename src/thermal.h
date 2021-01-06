#ifndef __THERMAL_H__
#define __THERMAL_H__

#include <Particle.h>

class Thermal
{
public:
    static void setup();
    static void update();

private:
    static bool sensing;
};

#endif
