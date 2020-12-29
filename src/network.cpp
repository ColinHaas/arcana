#define SYSLOG_DEBUG
#include <psyslog.h>

#include "config.h"
#include "network.h"

unsigned long Network::updated = 0;
unsigned long Network::synchronized = 0;
bool Network::configured = false;

bool Network::setup()
{
    // wait for network or configure new connection
    if (waitFor(WiFi.ready, TIMEOUT_WIFI_READY))
    {
        // configure network connections once available
        update();
    }
    else
    {
        // enable listening mode
        WiFi.listen();
    }

    return configured;
}

void Network::update()
{
    if (WiFi.ready())
    {
        // configure network connections if not already configured
        if (!configured)
        {
            // initialize connection to syslog server
            syslog_initialize(SERVER_URL, 514);

            configured = true;
        }

        // synchronize time with cloud once per day
        if (millis() - synchronized > TIME_SYNC_INTERVAL)
        {
            if (Particle.syncTime())
            {
                synchronized = millis();
            }
        }

        updated = millis();
    }
    else
    {
        configured = false;

        // enable listening mode if unable to connect to network
        if (!(WiFi.listening() || WiFi.connecting()))
        {
            WiFi.listen();
        }
    }
}
