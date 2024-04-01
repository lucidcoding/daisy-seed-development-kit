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
        void Start(uint32_t);
        uint64_t GetLedState();
        void Process(uint32_t, uint32_t);
        void OnKeyPressed(uint32_t);
        //void OnKeyReleased(uint32_t, uint32_t);
        uint8_t GetStateCode();
        void MoveToStep(uint8_t) {}
        void SetStepTimeUs(uint32_t);
        void SetTicksPerUs(uint32_t);
        void SyncPulse2ppqn();
        void SetSeqSyncSource(uint8_t);

    private:
        uint32_t stepTimeUs;
        uint32_t gateTimeUs;
        uint32_t ticksPerUs;
        StepIndicator stepIndicator;
        uint32_t lastStepStartTicks;
        uint8_t seqSyncSource;
        bool pulseOn;
        uint32_t lastPulseTicks;
        uint32_t ticksBetweenPulses;
        uint32_t playIntermediateNoteTicks;
        bool playIntermediaryNote;
        bool firstPulseReceived;
        void OnSavePatternPressed();
        void OnSeqSyncSelectPressed();
        void OnLoadPatternPressed();
        void OnClearPressed();
        void OnRecordPressed();
        void OnPlayPressed();
    };
}

#endif