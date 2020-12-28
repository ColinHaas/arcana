#ifndef __AUDIO_H___
#define __AUDIO_H___

#include <Particle.h>
#include <speaker.h>

class Audio
{
public:
    static bool setup();
    static bool update();

private:
    static Speaker speaker;
    static uint16_t amplitude;
    static uint32_t signal;

    static void sawtooth(uint16_t *buffer, uint16_t frequency);
};

#endif
