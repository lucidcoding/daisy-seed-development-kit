#pragma once
#ifndef STEP_SEQUENCER_STOP_STATE_H
#define STEP_SEQUENCER_STOP_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"
#include "StepIndicator.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StopState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void CheckForClockEvent(uint32_t currentTicks);
        void OnKeyPressed(uint32_t keyState);

    private:
        StepIndicator stepIndicator;
        void OnPlayPressed();
    };
}

#endif