#pragma once
#ifndef STEP_SEQUENCER_PLAY_STATE_H
#define STEP_SEQUENCER_PLAY_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"
#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class PlayState : public IState
    {
    public:
        uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t);

    private:
    };
}

#endif