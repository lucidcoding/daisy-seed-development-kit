#pragma once
#ifndef STEP_SEQUENCER_LEDS_H
#define STEP_SEQUENCER_LEDS_H

#define STEP_SEQUENCER_LEDS_I2C_ADDRESS 0x20
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
            {17, 28, 29, 3, 1, STEP_SEQUENCER_LEDS_NOT_USED},
            {18, 27, 30, 6, 0, 16},
            {19, 26, 31, 8, 2, 14},
            {20, 25, 32, 10, 4, 12},
            {21, 24, 33, 13, 5, 11},
            {22, 23, 34, 15, 7, 9}};
        uint32_t lastTicks;
        uint32_t ticksPerUs;
    };
}

#endif