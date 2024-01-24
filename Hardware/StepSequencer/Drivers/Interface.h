#pragma once
#ifndef STEP_SEQUENCER_INTERFACE_H
#define STEP_SEQUENCER_INTERFACE_H

#include "Keys.h"
#include "Leds.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

    class Interface
    {
    public:
        void Init();
        uint32_t ScanNextKeysColumn(uint32_t currentProcessTimeUs);
        void ScanNextLedsColumn(uint64_t newLedsState);
    private:
        Keys keys;
        Leds leds;
    };
}

#endif