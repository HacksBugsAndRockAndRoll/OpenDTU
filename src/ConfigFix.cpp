#include <ConfigFix.h>
#include <Hoymiles.h>
#include <Configuration.h>
ConfigFixClass ConfigFix;

void ConfigFixClass::init()
{
}
void ConfigFixClass::loop()
{
    if (millis() - _lastPoll > (_pollInterval * 1000))
    {
        CONFIG_T &config = Configuration.get();
        for (uint8_t i = 0; i < INV_MAX_COUNT; i++)
        {
            if (config.Inverter[i].Serial > 0)
            {
                auto inv = Hoymiles.getInverterBySerial(config.Inverter[i].Serial);
                if (inv == nullptr)
                {
                    ESP.restart();
                }
            }
        }
        _lastPoll = millis();
    }
}