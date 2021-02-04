#include <Adafruit_MPR121.h>

#include "buffer.h"
#include "config.h"
#include "network.h"
#include "touch.h"

void interrupt();

bool Touch::sensing = false;

uint16_t data[PROXIMITY_ELECTRODE_COUNT];
Adafruit_MPR121 mpr = Adafruit_MPR121();
volatile bool trigger = false;

void Touch::setup()
{
    if (wired(MPR121_I2C_ADDRESS))
    {
        if (mpr.begin(MPR121_I2C_ADDRESS))
        {
            // disable electrodes during configuration
            mpr.writeRegister(MPR121_ECR, 0x0);
            // set interrupt trigger thresholds
            mpr.setThreshholds(100, 200);
            // configure first filter samples and global charge current
            mpr.writeRegister(MPR121_CONFIG1, 0b11100000); // 34 samples, 32 uA
            // set global charge/discharge time and second filter configuration
            mpr.writeRegister(MPR121_CONFIG2, 0b00111000); // 18 samples, 0.5 μs
            // enable all electrodes
            mpr.writeRegister(MPR121_ECR, 0b10001111);

            // pinMode(TOUCH_IRQ_PIN, INPUT_PULLUP);
            // attachInterrupt(TOUCH_IRQ_PIN, interrupt, FALLING);

            sensing = true;
        }
    }

    if (!sensing)
        Serial.printlnf("<ERROR> MPR121");
}

void Touch::update()
{
    static uint32_t updated;
    if (trigger || ((millis() - updated) >= UPDATE_INTERVAL_TOUCH))
    {
        updated = millis();

        // reset sensor interrupt trigger
        trigger = false;

        if (sensing)
        {
            // refresh sensor data cache with electrode pin offset
            for (int i = 0; i < PROXIMITY_ELECTRODE_COUNT; i++)
            {
                data[i] = mpr.filteredData(i + PROXIMITY_ELECTRODE_PIN_OFFSET);
            }

            // report proximity values to server
            Network::proximity(data);
        }
    }
}

void interrupt()
{
    trigger = true;
}
