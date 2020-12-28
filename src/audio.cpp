#define SYSLOG_DEBUG
#include <psyslog.h>

#include "audio.h"
#include "config.h"

SYSTEM_THREAD(ENABLED);

Speaker Audio::speaker = Speaker(AUDIO_BUFFER_SIZE);
uint16_t Audio::amplitude = 50000; // (0-65535)
uint32_t Audio::signal = 0;

bool Audio::setup()
{
    // write initial data in the audio buffer (avoid initial silence)
    sawtooth(speaker.getBuffer(), AUDIO_TEST_FREQUENCY);

    // start audio output
    speaker.begin(AUDIO_SAMPLING_FREQUENCY);

    delay(AUDIO_TEST_TIME);

    // stop audio output
    speaker.end();

    LOGD("ready");
    return true;
}

bool Audio::update()
{
    if (speaker.ready())
    {
        // write new audio into buffer
        // sawtooth(speaker.getBuffer(), AUDIO_TEST_FREQUENCY);

        return true;
    }

    return false;
}

// fill buffer with generated sawtooth waveform
void Audio::sawtooth(uint16_t *buffer, uint16_t frequency)
{
    uint32_t increment = ((uint32_t)frequency * amplitude) / AUDIO_SAMPLING_FREQUENCY;

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
