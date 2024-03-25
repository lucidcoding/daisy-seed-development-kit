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

// #include "daisy_seed.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class Controller : public IController
    {
    public:
        void Init(uint32_t);
        void SetHardware(IHardware *);
        void SetKeyState(uint32_t);
        void Process(uint32_t);
        bool GetGate();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();
        uint8_t GetNote();
        uint64_t GetLedState();
        void SetTempo(uint8_t);
        void SetState(uint8_t);

        IState *state;
        BlinkState blinkState;
        StopState stopState;
        LoadState loadState;
        PlayState playState;
        SaveState saveState;
        StepRecState stepRecState;
        SetSeqSyncState setSeqSyncState;
        void SetCurrentStepIndex(uint8_t newCurrentStepIndex) { currentStepIndex = newCurrentStepIndex; }
        void SetGate(bool newGate) { gate = newGate; }
        void ToggleSeqSyncSource();
        void SwitchToBlinkState(uint64_t);
        void SwitchToLoadState(IState *);
        void MoveBackStep();
        void MoveNextStep();
        void MoveToFirstStep();

        // For testing only
        void EnterTestMode();
        uint8_t GetCurrentStepIndex() { return currentStepIndex; }
        uint8_t GetMode() { return mode; }
        void SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT]);
        Step *GetSteps() { return steps; }
        Step *GetSavedPatterns() { return savedPatterns; }
        uint8_t GetSeqSyncSource() { return seqSyncSource; }
        // daisy::DaisySeed *daisy;

    private:
        Step savedPatterns[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT * 8];
        Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT];
        uint8_t currentStepIndex;
        uint8_t mode;
        uint8_t tempo;
        bool gate;
        uint64_t ledState;
        uint32_t lastKeyState;
        uint8_t seqSyncSource;
        IHardware *hardware;
        void ClearSteps();
        void UpdateLedStates();
        uint8_t GetNoteFromKeyPressed(uint32_t);
        uint8_t GetNoteFromPatternIndex(uint8_t);
        uint8_t GetPatternIndexFromNote(uint8_t);
        void ActivateCurrentStep();
        void SavePattern(uint8_t);
        void LoadPattern(uint8_t);
    };
}

#endif