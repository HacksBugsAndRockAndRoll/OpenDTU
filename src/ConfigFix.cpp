#include <ConfigFix.h>
#include <Hoymiles.h>
#include <Configuration.h>
ConfigFixClass ConfigFix;

void ConfigFixClass::init()
{
      _consecFix = 0;
}
void ConfigFixClass::loop()
{
    boolean broken = false;
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
                    broken = true;
                    break;
                }
            }
        }
        if (broken)
        {
            _consecFix++;
            if(_consecFix > 3){
                ESP.restart();
            }
            // clear inverters
            Serial.println(F("  clearing inverters..."));
            for (uint8_t i = 0; i < Hoymiles.getNumInverters(); i++)
            {
                Hoymiles.removeInverterByPos(i);
            }
            // re build inverters in hoymiles
            Serial.println(F("  re-filling inverters..."));
            for (uint8_t i = 0; i < INV_MAX_COUNT; i++)
            {
                if (config.Inverter[i].Serial > 0)
                {
                    Serial.print(F("  adding inverter: "));
                    Serial.print(config.Inverter[i].Serial, HEX);
                    auto inv = Hoymiles.addInverter(
                        config.Inverter[i].Name,
                        config.Inverter[i].Serial);
                    for (uint8_t c = 0; c < INV_MAX_CHAN_COUNT; c++)
                    {
                        inv->Statistics()->setChannelMaxPower(c, config.Inverter[i].MaxChannelPower[c]);
                    }
                    Serial.println(F(" done"));
                }
            }
        }else{
            _consecFix = 0;
        }
        _lastPoll = millis();
    }
}