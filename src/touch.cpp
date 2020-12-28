#define SYSLOG_DEBUG
#include <psyslog.h>

#include "touch.h"
#include "config.h"
#include "buffer.h"

volatile bool Touch::trigger = false;
Adafruit_MPR121 Touch::mpr = Adafruit_MPR121();
uint16_t Touch::data[PROXIMITY_ELECTRODE_COUNT];

bool Touch::setup()
{
  if (!mpr.begin(0x5A))
  {
    LOGC("error");
    return false;
  }

  // disable electrodes during configuration
  mpr.writeRegister(MPR121_ECR, 0x0);

  // set interrupt trigger thresholds
  mpr.setThreshholds(100, 200);

  // configure first filter samples and global charge current
  mpr.writeRegister(MPR121_CONFIG1, 0b11100000); // 34 samples, 32 uA
  // configure global charge/discharge time, second filter samples, and

  mpr.writeRegister(MPR121_CONFIG2, 0b00111000); // 18 samples, 0.5 μs

  // enable all electrodes
  mpr.writeRegister(MPR121_ECR, 0b10001111);

  // pinMode(TOUCH_IRQ_PIN, INPUT_PULLUP);
  // attachInterrupt(TOUCH_IRQ_PIN, interrupt, FALLING);

  LOGD("ready");
  return true;
}

void Touch::update()
{
  // check if interrupt has been triggered by sensor
  if (trigger)
  {
    trigger = false;
    LOGD("trigger");
  }

  // refresh sensor data cache with electrode pin offset
  for (int i = 0; i < PROXIMITY_ELECTRODE_COUNT; i++)
  {
    data[i] = mpr.filteredData(i + PROXIMITY_ELECTRODE_PIN_OFFSET);
  }

  // log sensor values at fixed interval
  static uint32_t updated = 0;
  if ((millis() - updated) > 500)
  {
    LOGD("0<%u> 1<%u> 2<%u> 3<%u> 4<%u> 5<%u> 6<%u> 7<%u>", data[0], data[1],
         data[2], data[3], data[4], data[5], data[6], data[7]);
    updated = millis();
  }
}

void Touch::interrupt() { trigger = true; }
