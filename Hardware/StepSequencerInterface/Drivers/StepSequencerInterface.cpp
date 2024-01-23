#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "StepSequencerInterface.h"

namespace developmentKit::stepSequencer
{
    void StepSequencerInterface::Init()
    {
        leds.Init();
        keys.Init();
    }

    uint32_t StepSequencerInterface::ScanNextKeysColumn(uint32_t currentProcessTimeUs)
    {
        return keys.ScanNextColumn(currentProcessTimeUs);
    }

    void StepSequencerInterface::ScanNextLedsColumn(uint64_t newLedsState)
    {
        leds.SetLeds(newLedsState);
        leds.ScanNextColumn();
    }
}