#ifndef __MOTION_H___
#define __MOTION_H___

#include <Particle.h>

class Motion
{
  public:
    static void setup();
    static void update();

  private:
    static volatile clock_t detected;

    static void interrupt();
};

#endif
