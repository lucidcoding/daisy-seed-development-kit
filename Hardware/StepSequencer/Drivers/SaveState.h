#pragma once
#ifndef STEP_SEQUENCER_SAVE_STATE_H
#define STEP_SEQUENCER_SAVE_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class SaveState : public IState
    {
    public:
        uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t);
        void CheckForClockEvent(uint32_t currentTicks);
        void OnKeyPressed(uint32_t keyState);

    private:
    };
}

#endif