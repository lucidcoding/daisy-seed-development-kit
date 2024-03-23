#pragma once
#ifndef STEP_SEQUENCER_ISTATE_H
#define STEP_SEQUENCER_ISTATE_H

#include "stdint.h"
#include "Constants.h"
#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class IState
    {
    public:
        virtual uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t) = 0;

    private:
    };
}

#endif