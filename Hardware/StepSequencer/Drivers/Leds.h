#pragma once
#ifndef STEP_SEQUENCER_LEDS_H
#define STEP_SEQUENCER_LEDS_H

#define STEP_SEQUENCER_LEDS_SCAN_INTERVAL_US 700
#define STEP_SEQUENCER_LEDS_NOT_USED 255
#define STEP_SEQUENCER_LEDS_NUMBER_OF_COLUMNS 6
#define STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS 6

#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

    class Leds
    {
    public:
        void Init();
        void Scan(uint64_t, uint32_t);

    private:
        Mcp23017 mcp;
        uint64_t state;
        uint8_t currentColumnIndex;
        uint8_t columnPins[STEP_SEQUENCER_LEDS_NUMBER_OF_COLUMNS] = {8, 9, 10, 11, 12, 13};
        uint8_t rowPins[STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS] = {1, 4, 5, 6};
        uint8_t ledLookup[STEP_SEQUENCER_LEDS_NUMBER_OF_COLUMNS][STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS] = {
            {22, 10, 0, 11},
            {21, 9, 1, 12},
            {20, 8, 2, 13},
            {19, 7, 3, 14},
            {18, 6, 4, 15},
            {17, 5, STEP_SEQUENCER_LEDS_NOT_USED, 16}};
        uint32_t lastTicks;
        uint32_t ticksPerUs;
    };
}

#endif