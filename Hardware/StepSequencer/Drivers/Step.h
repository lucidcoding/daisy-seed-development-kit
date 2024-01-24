#pragma once
#ifndef STEP_SEQUENCER_STEP_H
#define STEP_SEQUENCER_STEP_H

#include "stdint.h"

namespace developmentKit::stepSequencer
{
    class Step
    {
    public:
        uint8_t note;
        bool octaveUp;
        bool octaveDown;
        bool gate;
        bool slide;
        bool accent;

    private:
    };
}

#endif