#include <Particle.h>

#include "audio.h"
#include "config.h"
#include "display.h"
#include "motion.h"
#include "network.h"
#include "thermal.h"
#include "touch.h"

SYSTEM_THREAD(ENABLED);

void setup()
{
    delay(2000);

    Serial.begin();
    waitUntil(Serial.isConnected);
    Serial.println();

    pinMode(BUILTIN_LED_PIN, OUTPUT);

    Network::setup();
    Audio::setup();
    Thermal::setup();
    Display::setup();
    Touch::setup();
    Motion::setup();

    Serial.println("<START>");
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

    RGB.color(RGB_STATUS_MOTION);
    Motion::update();
}
