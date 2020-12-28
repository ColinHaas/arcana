#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <Particle.h>
#include <Adafruit_MPR121.h>

class Touch
{
public:
  static bool setup();
  static void update();

private:
  static uint16_t data[];
  static Adafruit_MPR121 mpr;
  static volatile bool trigger;

  static void interrupt();
};

#endif