#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Particle.h>

class Display
{
public:
  static void setup();
  static void update();

private:
  static bool sensing;

  static void test();
};

#endif