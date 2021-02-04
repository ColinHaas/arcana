#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <Particle.h>

class Touch
{
  public:
    static void setup();
    static void update();

  private:
    static bool sensing;
};

#endif