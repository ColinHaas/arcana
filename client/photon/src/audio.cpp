#include <speaker.h>

#include "audio.h"
#include "config.h"

Speaker speaker = Speaker(AUDIO_BUFFER_SIZE);
uint16_t amplitude = 50000; // (0-65535)
uint32_t signal = 0;

void Audio::setup()
{
    // write initial data in the audio buffer (avoid initial silence)
    sawtooth(speaker.getBuffer(), AUDIO_TEST_FREQUENCY);

    // start audio output
    speaker.begin(AUDIO_SAMPLING_FREQUENCY);

    delay(AUDIO_TEST_TIME);

    // stop audio output
    speaker.end();
}

void Audio::update()
{
    if (speaker.ready())
    {
        // write new audio into buffer
        // sawtooth(speaker.getBuffer(), AUDIO_TEST_FREQUENCY);
    }
}

void Audio::sawtooth(uint16_t *buffer, uint16_t frequency)
{
    uint32_t increment = ((uint32_t)frequency * amplitude) / AUDIO_SAMPLING_FREQUENCY;

    // fill buffer with generated sawtooth waveform
    for (uint16_t i = 0; i < AUDIO_BUFFER_SIZE; i++)
    {
        buffer[i] = signal;
        signal += increment;
        if (signal > amplitude)
        {
            signal -= amplitude;
        }
    }
}
