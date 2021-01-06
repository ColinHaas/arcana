#include <Particle.h>

#include "config.h"
#include "audio.h"
#include "display.h"
#include "touch.h"
#include "thermal.h"
#include "network.h"

SYSTEM_THREAD(ENABLED);

void setup()
{
  delay(2000);

#ifdef SERIAL_DEBUG
  Serial.begin();
  waitUntil(Serial.isConnected);
  Serial.println();
#endif

  pinMode(BUILTIN_LED_PIN, OUTPUT);

  Network::setup();
  Audio::setup();
  Thermal::setup();
  Display::setup();
  Touch::setup();
}

void loop()
{
  if (WiFi.ready())
  {
    RGB.control(true);
    RGB.brightness(STATUS_LED_BRIGHTNESS);
    RGB.color(RGB_COLOR_GREY);
  }
  else
  {
    RGB.control(false);
  }

  RGB.color(RGB_STATUS_NETWORK);
  Network::update();

  RGB.color(RGB_STATUS_TOUCH);
  Touch::update();

  RGB.color(RGB_STATUS_THERMAL);
  Thermal::update();

  RGB.color(RGB_STATUS_DISPLAY);
  Display::update();

  RGB.color(RGB_STATUS_AUDIO);
  Audio::update();
}
