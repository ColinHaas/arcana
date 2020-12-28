#define SYSLOG_DEBUG
#include <psyslog.h>

#include "network.h"
#include "config.h"

unsigned long Network::synchronized = 0;

bool Network::setup()
{
    if (waitFor(WiFi.ready, TIMEOUT_WIFI_READY))
    {
        // initialize connection to syslog server
        syslog_initialize(SERVER_URL, 514);

        LOGD("ready");
        return true;
    }
    return false;
}

void Network::update()
{
    // synchronize time with cloud once per day
    if (millis() - synchronized > (24 * 60 * 60 * 1000))
    {
        Particle.syncTime();
        synchronized = millis();
    }
}
