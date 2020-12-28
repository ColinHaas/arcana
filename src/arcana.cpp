#include <Particle.h>

#include "config.h"
#include "audio.h"
#include "display.h"
#include "touch.h"
#include "thermal.h"
#include "network.h"

void setup()
{
  delay(2000);

  Network::setup();
  Audio::setup();
  Thermal::setup();
  Display::setup();
  Touch::setup();
}

void loop()
{
  Network::update();
  Audio::update();
  Thermal::update();
  Display::update();
  Touch::update();

  Particle.process();
}
