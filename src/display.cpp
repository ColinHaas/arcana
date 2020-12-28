#define SYSLOG_DEBUG
#include <psyslog.h>

#include "display.h"
#include "config.h"
#include "graphics.h"

Adafruit_TSL2561_Unified Display::tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 2561);
Adafruit_NeoPixel Display::output = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, SK6812RGBW);
bool Display::ready = false;
uint32_t Display::lux = 0;
uint32_t Display::updated = 0;
uint32_t Display::reported = 0;

void Display::setup()
{
  output.setBrightness(32);
  output.begin();
  output.show();

  if (tsl.begin())
  {
    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);

    LOGD("ready");
    ready = true;
  }
}

bool Display::update()
{
  if (!ready)
  {
    LOGE("error");
    return false;
  }

  static uint32_t updated;
  if ((millis() - updated) >= PIXEL_UPDATE_INTERVAL)
  {
    // update ambient lux reading
    uint16_t broadband;
    uint16_t ir;
    tsl.getLuminosity(&broadband, &ir);
    lux = tsl.calculateLux(broadband, ir);

    // log ambient lux values at fixed interval
    if ((millis() - reported) > DISPLAY_REPORT_INTERVAL)
    {
      LOGD("lux=%u", lux);
      reported = millis();
    }

    test();
    output.show();

    updated = millis();
    return true;
  }

  return false;
}

void Display::test()
{
  static int marker = 0;
  static int increment = 1;
  static uint32_t test_color = output.Color(random(256), random(256), random(256));
  output.clear();
  for (uint16_t i = 0; i < output.numPixels(); i++)
  {
    output.setPixelColor(i, (i <= marker) ? test_color : output.Color(0, 0, 0, 0));
  }
  marker += increment;
  if ((marker == (output.numPixels() - 1)) || (marker == 0))
  {
    increment *= (-1);
  }
}
