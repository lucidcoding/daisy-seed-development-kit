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
        void SetStepTimeUs(uint32_t);
        void SetTicksPerUs(uint32_t);

    private:
        uint32_t stepTimeUs;
        uint32_t gateTimeUs;
        uint32_t ticksPerUs;
        StepIndicator stepIndicator;
        bool playJustPressed;
        uint32_t lastStepStartTicks;
        void OnSavePatternPressed();
        void OnSeqSyncSelectPressed();
        void OnLoadPatternPressed();
        void OnClearPressed();
        void OnRecordPressed();
        void OnPlayPressed();
    };
}

#endif