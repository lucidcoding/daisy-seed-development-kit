#pragma once
#ifndef STEP_SEQUENCER_STEP_INDICATOR_H
#define STEP_SEQUENCER_STEP_INDICATOR_H

#include "stdint.h"
#include "Constants.h"
#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StepIndicator
    {
    public:
        uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t);

    private:
    };
}

#endif