#pragma once
#ifndef STEP_SEQUENCER_KEYS_H
#define STEP_SEQUENCER_KEYS_H

#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

#define STEP_SEQUENCER_KEYS_SCAN_INTERVAL_US 350
#define STEP_SEQUENCER_KEYS_NUMBER_OF_KEYS 23
#define STEP_SEQUENCER_KEYS_NUMBER_OF_COLUMNS 6
#define STEP_SEQUENCER_KEYS_NUMBER_OF_ROWS 4
#define STEP_SEQUENCER_KEYS_NO_KEY_PRESS 255
#define STEP_SEQUENCER_KEYS_NOT_USED 255

namespace developmentKit::hardware::stepSequencer::drivers
{
    using namespace daisysp;
    using namespace daisy;

    class Keys
    {
    public:
        void Init();
        uint32_t ScanNextColumn(uint32_t);

    private:
        Mcp23017 mcp;
        uint8_t currentColumnIndex;
        uint8_t columnPins[STEP_SEQUENCER_KEYS_NUMBER_OF_COLUMNS] = {13, 12, 11, 10, 9, 8};
        uint8_t rowPins[STEP_SEQUENCER_KEYS_NUMBER_OF_ROWS] = {4, 5, 6, 7};
        uint8_t keyLookup[STEP_SEQUENCER_KEYS_NUMBER_OF_COLUMNS][STEP_SEQUENCER_KEYS_NUMBER_OF_ROWS] = {
            {18, 4, 5, STEP_SEQUENCER_KEYS_NOT_USED},
            {17, 2, 7, 19},
            {15, 0, 9, 20},
            {13, 1, 11, 22},
            {10, 3, 12, 21},
            {8, 6, 14, 16}};
        uint32_t state;
        uint32_t lastTicks;
        uint32_t ticksPerUs;
        uint8_t debounceBuffer[STEP_SEQUENCER_KEYS_NUMBER_OF_KEYS];
    };
}

#endif