#ifndef __NETWORK_H___
#define __NETWORK_H___

#include <Particle.h>

class Network
{
public:
    static bool setup();
    static void update();

private:
    static unsigned long synchronized;
};

#endif
