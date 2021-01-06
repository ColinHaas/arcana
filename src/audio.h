#ifndef __AUDIO_H___
#define __AUDIO_H___

#include <Particle.h>

class Audio
{
public:
    static void setup();
    static void update();

private:
    static void sawtooth(uint16_t *buffer, uint16_t frequency);
};

#endif
