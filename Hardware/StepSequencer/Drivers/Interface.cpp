#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "Interface.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void Interface::Init()
    {
        leds.Init();
        keys.Init();
    }

    uint32_t Interface::ScanKeys(uint32_t currentProcessTimeUs)
    {
        return keys.ScanNextColumn(currentProcessTimeUs);
    }

    void Interface::ScanLeds(uint64_t newLedState, uint32_t currentProcessTimeUs)
    {
        leds.Scan(newLedState, currentProcessTimeUs);
    }
}