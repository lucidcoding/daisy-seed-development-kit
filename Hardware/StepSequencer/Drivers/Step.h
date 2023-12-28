#pragma once
#ifndef STEP_SEQUENCER_STEP_H
#define STEP_SEQUENCER_STEP_H

#include "daisysp.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;

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