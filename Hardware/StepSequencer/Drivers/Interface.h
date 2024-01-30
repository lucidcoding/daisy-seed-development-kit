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
        uint32_t ScanKeys(uint32_t);
        void ScanLeds(uint64_t, uint32_t);
    private:
        Keys keys;
        Leds leds;
    };
}

#endif