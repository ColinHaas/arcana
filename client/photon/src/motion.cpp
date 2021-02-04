#include "motion.h"
#include "config.h"

volatile clock_t Motion::detected = 0;

void Motion::setup()
{
    pinMode(RADAR_SIGNAL_PIN, INPUT);

    attachInterrupt(RADAR_SIGNAL_PIN, interrupt, RISING);
}

void Motion::update()
{
    static clock_t triggered = 0;
    static bool signal = (digitalRead(RADAR_SIGNAL_PIN) == HIGH);

    if (detected > triggered)
    {
        signal = true;
        triggered = detected;

        Serial.println("<MOTION_DETECTED>");
    }

    if (signal && digitalRead(RADAR_SIGNAL_PIN) == LOW)
    {
        signal = false;

        Serial.println("<MOTION_RESET>");
    }
}

void Motion::interrupt()
{
    // set motion detect flag
    detected = millis();
}
