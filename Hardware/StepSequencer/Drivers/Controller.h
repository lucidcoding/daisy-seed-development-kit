#pragma once
#ifndef STEP_SEQUENCER_CONTROLLER_H
#define STEP_SEQUENCER_CONTROLLER_H

#include "stdint.h"
#include "Constants.h"
#include "Step.h"
#include "IController.h"
#include "IHardware.h"
#include "IState.h"
#include "BlinkState.h"
#include "LoadState.h"
#include "PlayState.h"
#include "SaveState.h"
#include "SetSeqSyncState.h"
#include "StepRecState.h"
#include "StopState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class Controller : public IController
    {
    public:
        Controller(BlinkState *blinkState,
                   StopState *stopState,
                   LoadState *loadState,
                   PlayState *playState,
                   SaveState *saveState,
                   StepRecState *stepRecState,
                   SetSeqSyncState *setSeqSyncState);
        void Init(uint32_t);
        void SetHardware(IHardware *);
        void Process(uint32_t, uint32_t);
        bool GetGate();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();
        uint8_t GetNote();
        uint64_t GetLedState();
        void SetTempo(uint8_t);
        void SetCurrentStepIndex(uint8_t newCurrentStepIndex) { currentStepIndex = newCurrentStepIndex; }
        void SetGate(bool newGate) { gate = newGate; }
        void ToggleSeqSyncSource();
        void SwitchToBlinkState(uint64_t);
        void SwitchToLoadState(IState *);
        void SwitchToPlayStateAndRestart(uint32_t);
        void SwitchToPlayStateAndContinue();
        void SwitchToStopState();
        void SwitchToStepRecState();
        void SwitchToSaveState();
        void SwitchToSetSeqSyncState();
        void MoveBackStep();
        void MoveNextStep();
        void MoveToFirstStep();
        void SyncPulse2ppqn();

        // For testing only
        void EnterTestMode();
        uint8_t GetCurrentStepIndex() { return currentStepIndex; }
        uint8_t GetMode() { return state->GetStateCode(); }
        void SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT]);
        Step *GetSteps() { return steps; }
        Step *GetSavedPatterns() { return savedPatterns; }
        uint8_t GetSeqSyncSource() { return seqSyncSource; }

    private:
        IState *state;
        BlinkState *_blinkState;
        StopState *_stopState;
        LoadState *_loadState;
        PlayState *_playState;
        SaveState *_saveState;
        StepRecState *_stepRecState;
        SetSeqSyncState *_setSeqSyncState;
        Step savedPatterns[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT * 8];
        Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT];
        uint8_t currentStepIndex;
        uint8_t tempo;
        bool gate;
        uint64_t ledState;
        uint8_t seqSyncSource;
        IHardware *hardware;
        void ClearSteps();
        void UpdateLedStates();
        void ActivateCurrentStep();
        void SavePattern(uint8_t);
        void LoadPattern(uint8_t);
    };
}

#endif