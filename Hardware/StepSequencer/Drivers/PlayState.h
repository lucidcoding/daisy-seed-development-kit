#pragma once
#ifndef STEP_SEQUENCER_PLAY_STATE_H
#define STEP_SEQUENCER_PLAY_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"
#include "StepIndicator.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class PlayState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void CheckForClockEvent(uint32_t);
        void OnKeyPressed(uint32_t);
        void OnKeyReleased(uint32_t, uint32_t);

    private:
        StepIndicator stepIndicator;
        bool playJustPressed;
        uint32_t lastStepStartTicks;
        void OnSavePatternPressed();
        void OnSeqSyncSelectPressed();
        void OnClearPressed();
        void OnRecordPressed();
        void OnPlayPressed();
    };
}

#endif