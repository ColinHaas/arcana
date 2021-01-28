#include <Adafruit_TSL2561_U.h>
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#include <neopixel.h>

#include "config.h"
#include "display.h"
#include "graphics.h"
#include "network.h"

bool Display::sensing = false;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_I2C_ADDRESS, 2561);
Adafruit_NeoPixel output = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, SK6812RGBW);

void Display::setup()
{
    output.setBrightness(DISPLAY_BRIGHTNESS_DEFAULT);
    output.begin();
    output.show();

    if (wired(TSL2561_I2C_ADDRESS))
    {
        if (tsl.begin())
        {
            tsl.enableAutoRange(true);
            tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);

            sensing = true;
        }
    }

    if (!sensing)
        Serial.printlnf("<ERROR> TSL2561");
}

void Display::update()
{
    static uint32_t updated;
    if ((millis() - updated) >= UPDATE_INTERVAL_DISPLAY)
    {
        updated = millis();

        if (sensing)
        {
            // update ambient lux reading
            uint16_t broadband;
            uint16_t ir;
            tsl.getLuminosity(&broadband, &ir);
            uint32_t lux = tsl.calculateLux(broadband, ir);

            // report lux reading to server
            Network::lighting(lux);
        }

        test();

        output.show();
    }
}

void Display::test()
{
    static int marker = 0;
    static int increment = 1;

    CHSV hsv = CHSV(random(256), 255, 255);
    CRGB rgb;
    hsv2rgb_rainbow(hsv, rgb);
    static uint32_t color = output.Color(rgb.red, rgb.green, rgb.blue);

    output.clear();
    for (uint16_t i = 0; i < output.numPixels(); i++)
    {
        output.setPixelColor(i, (i <= marker) ? color : output.Color(0, 0, 0, 0));
    }
    marker += increment;
    if ((marker == (output.numPixels() - 1)) || (marker == 0))
    {
        increment *= (-1);
    }
}
