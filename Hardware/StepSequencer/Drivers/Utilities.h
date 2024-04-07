#pragma once
#ifndef STEP_SEQUENCER_UTILITIES_H
#define STEP_SEQUENCER_UTILITIES_H

#include "stdint.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class Utilities
    {
    public:
        static uint8_t GetNoteFromKeyPressed(uint32_t);
        static uint8_t GetNoteFromPatternIndex(uint8_t);
        static uint8_t GetPatternIndexFromNote(uint8_t);

    private:
        static uint8_t lookup[8];
    };
}

#endif