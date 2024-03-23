#pragma once
#ifndef STEP_SEQUENCER_STEP_REC_STATE_H
#define STEP_SEQUENCER_STEP_REC_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"
#include "StepIndicator.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StepRecState : public IState
    {
    public:
        uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t);

    private:
        StepIndicator stepIndicator;
    };
}

#endif