#pragma once
#ifndef STEP_SEQUENCER_LEDS_H
#define STEP_SEQUENCER_LEDS_H

#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"
#include "ILeds.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

    class Leds : public ILeds
    {
    public:
        void Init();
        void SetLed(uint8_t ledIndex, bool state);
        void ScanNextColumn();

    private:
        Mcp23017 mcp;
        bool ledStates[64];
        uint8_t currentColumnIndex;
        uint8_t columnPins[6] = {8, 9, 10, 11, 12, 13};
        uint8_t rowPins[4] = {1, 4, 5, 6};
        uint8_t ledLookup[6][4] = {
            {22, 10, 0, 11},
            {21, 9, 1, 12},
            {20, 8, 2, 13},
            {19, 7, 3, 14},
            {18, 6, 4, 15},
            {17, 5, 255, 16}};
    };
}

#endif