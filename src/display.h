#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Particle.h>
#include <Adafruit_TSL2561_U.h>
#include <neopixel.h>

class Display
{
public:
  static void setup();
  static bool update();

private:
  static Adafruit_TSL2561_Unified tsl;
  static Adafruit_NeoPixel output;
  static bool ready;
  static uint32_t lux;
  static uint32_t updated;
  static uint32_t reported;

  static void test();
};

#endif