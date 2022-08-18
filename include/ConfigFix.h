#pragma once

#include "types.h"

class ConfigFixClass
{
    public:
        void init();
        void loop();
    private:
        uint32_t _pollInterval = 10;
        uint32_t _lastPoll = 0;
        uint8_t _consecFix = 0;
};
extern ConfigFixClass ConfigFix;