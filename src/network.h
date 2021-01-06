#ifndef __NETWORK_H___
#define __NETWORK_H___

#include <Particle.h>

class Network
{
public:
    static const char *ID;

    static void setup();
    static void update();
    static bool connected();

    static void heartbeat();
    static void temperature(float value);
    static void lighting(uint32_t value);
    static void proximity(const uint16_t *values);

private:
    static void synchronize();
    static bool send(const char *topic, const char *message);
    static bool send(const char *topic, const byte *data, unsigned int length);

    template <typename T>
    static bool send(const char *topic, T value)
    {
        return send(topic, String(value).c_str());
    }

    template <typename T>
    static bool send(const char *topic, const T *values, const char *delimiter)
    {
        String message;
        for (unsigned int i = 0; i < (sizeof(values) / sizeof(T)); i++)
            message += (((i > 0) ? delimiter : "") + String(values[i]));
        return send(topic, message.c_str());
    }
};

#endif
