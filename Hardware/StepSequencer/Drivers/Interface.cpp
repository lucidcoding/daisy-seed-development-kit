#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "Interface.h"

namespace developmentKit::stepSequencer
{
    void Interface::Init()
    {
        leds.Init();
        keys.Init();
    }

    uint32_t Interface::ScanNextKeysColumn(uint32_t currentProcessTimeUs)
    {
        return keys.ScanNextColumn(currentProcessTimeUs);
    }

    void Interface::ScanNextLedsColumn(uint64_t newLedState, uint32_t currentProcessTimeUs)
    {
        leds.SetLeds(newLedState, currentProcessTimeUs);
    }
}