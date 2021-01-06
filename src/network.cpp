// #define DEBUG_MQTT_SERIAL_OUTPUT
#include <MQTT.h>

#include "config.h"
#include "network.h"

const char *Network::ID = System.deviceID().c_str();

void receive(char *topic, byte *payload, unsigned int length);

MQTT mqtt((uint8_t *)SERVER_IP_ADDRESS, MQTT_DEFAULT_PORT, receive);

void Network::setup()
{
#ifdef SERIAL_DEBUG
    waitFor(Particle.connected, CLOUD_CONNECT_TIMEOUT);
    Serial.printlnf("<ID> %s", ID);
    IPAddress ip = WiFi.localIP();
    Serial.printlnf("<LOCAL_IP> %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    uint8_t *mac = WiFi.macAddress(mac);
    Serial.printlnf("<MAC_ADDRESS> %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
}

void Network::update()
{
    if (WiFi.ready())
    {
        bool connected = mqtt.isConnected();

        if (!connected)
        {
            static unsigned long retry;
            if ((millis() - retry) > CONNECT_RETRY_INTERVAL)
            {
                retry = millis();

                connected = mqtt.connect(ID);
            }
        }

        if (connected)
        {
            mqtt.loop();

            heartbeat();
        }
    }
    else
    {
        // enable listening mode if unable to connect to network
        if (!(WiFi.listening() || WiFi.connecting()))
        {
            WiFi.listen();
        }
    }
}

bool Network::connected() { return mqtt.isConnected(); }

void Network::heartbeat()
{
    static const String topic = String::format("/device/%s/heartbeat", ID);

    static unsigned long reported;
    if ((millis() - reported) > REPORT_INTERVAL_HEARTBEAT)
    {
        String timestamp = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);

        if (send(topic, timestamp.c_str()))
        {
            reported = millis();
        }
    }
}

void Network::temperature(float value)
{
    static const String topic = String::format("/device/%s/temperature", ID);

    static unsigned long reported;
    if ((millis() - reported) > REPORT_INTERVAL_TEMPERATURE)
    {
        if (send(topic, value))
        {
            reported = millis();
        }
    }
}

void Network::lighting(uint32_t value)
{
    static const String topic = String::format("/device/%s/lighting", ID);

    static unsigned long reported;
    if ((millis() - reported) > REPORT_INTERVAL_LIGHTING)
    {
        if (send(topic, value))
        {
            reported = millis();
        }
    }
}

void Network::proximity(const uint16_t *values)
{
    static const String topic = String::format("/device/%s/proximity", ID);

    static unsigned long reported;
    if ((millis() - reported) > REPORT_INTERVAL_PROXIMITY)
    {
        if (send(topic.c_str(), values, " "))
        {
            reported = millis();
        }
    }
}

void Network::synchronize()
{
    if (Particle.connected() && (!Particle.syncTimePending()))
    {
        clock_t synchronized = Particle.timeSyncedLast();
        if ((millis() - synchronized) > TIME_SYNC_INTERVAL)
        {
            Particle.syncTime();
        }
    }
}

bool Network::send(const char *topic, const char *message)
{
    if (!mqtt.isConnected())
        return false;

    digitalWrite(BUILTIN_LED_PIN, HIGH);
    bool sent = mqtt.publish(topic, message);
    digitalWrite(BUILTIN_LED_PIN, LOW);

#ifdef SERIAL_DEBUG
    if (sent)
        Serial.printlnf("<SEND> %s -> %s", message, topic);
#endif

    return sent;
}

bool Network::send(const char *topic, const byte *data, unsigned int length)
{
    if (!mqtt.isConnected())
        return false;

    digitalWrite(BUILTIN_LED_PIN, HIGH);
    bool sent = mqtt.publish(topic, data, length);
    digitalWrite(BUILTIN_LED_PIN, LOW);

#ifdef SERIAL_DEBUG
    if (sent)
        Serial.printlnf("<SEND> [%u bytes] -> %s", length, topic);
#endif

    return sent;
}

void receive(char *topic, byte *payload, unsigned int length)
{
#ifdef SERIAL_DEBUG
    Serial.printlnf("<RECEIVE> [%u bytes] <- %s", length, topic);
#endif

    // TODO: handle incoming message data
}
