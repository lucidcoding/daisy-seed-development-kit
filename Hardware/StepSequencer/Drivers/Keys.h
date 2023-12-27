#pragma once
#ifndef STEP_SEQUENCER_KEYS_H
#define STEP_SEQUENCER_KEYS_H

#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

#define STEP_SEQUENCER_DEBOUNCE_TIME 1000

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

    class Keys
    {
    public:
        void Init();
        uint64_t Process();

    private:
        Mcp23017 mcp;
        uint8_t columnPinIndex;
        uint8_t columnPins[6] = {8, 9, 10, 11, 12, 13};
        uint8_t inputPins[4] = {0, 2, 3, 7};
        uint8_t switchLookup[6][4] = {
            {10, 11, 22, 0},
            {9, 12, 21, 1},
            {8, 13, 20, 2},
            {7, 14, 19, 3},
            {6, 15, 18, 4},
            {5, 16, 17, 255}};
        bool stableState[24];
        bool lastState[24];
        uint32_t lastDebounceTime[24];
    };
}

#endif