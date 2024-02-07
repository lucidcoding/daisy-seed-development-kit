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

namespace developmentKit::hardware::stepSequencer::drivers
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
        uint8_t rowPins[STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS] = {5, 6, 7, 4, 3, 2};
        uint8_t ledLookup[STEP_SEQUENCER_LEDS_NUMBER_OF_COLUMNS][STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS] = {
            {18, 29, 30, 4, 2, STEP_SEQUENCER_LEDS_NOT_USED},
            {19, 28, 31, 7, 1, 17},
            {20, 27, 32, 9, 3, 15},
            {21, 26, 33, 11, 5, 13},
            {22, 25, 34, 14, 6, 12},
            {23, 24, 35, 16, 8, 10}};
        uint32_t lastTicks;
        uint32_t ticksPerUs;
    };
}

#endif