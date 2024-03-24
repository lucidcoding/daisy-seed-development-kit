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
#include "PlayState.h"
#include "SetSeqSyncState.h"
#include "StepRecState.h"
#include "StopState.h"

//#include "daisy_seed.h"

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

        IState * state;
        BlinkState blinkState;
        StopState stopState;
        PlayState playState;
        StepRecState stepRecState;
        SetSeqSyncState setSeqSyncState;

        // For testing only
        void SetStepTime(uint32_t);
        uint8_t GetCurrentStepIndex() { return currentStepIndex; }
        uint8_t GetMode() { return mode; }
        void SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT]);
        Step *GetSteps() { return steps; }
        Step *GetSavedPatterns() { return savedPatterns; }
        uint32_t GetLastTicks() { return lastStepStartTicks; }
        void SetLastTicks(uint32_t newLastTicks) { lastStepStartTicks = newLastTicks; }
        uint8_t GetSeqSyncSource() { return seqSyncSource; }
        void SetBlinkTimeUs(uint32_t newBlinkTimeUs) { 
            blinkState.SetBlinkTimeUs(newBlinkTimeUs);
        }
        //daisy::DaisySeed *daisy;

    private:
        Step savedPatterns[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT * 8];
        Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT];
        uint8_t currentStepIndex;
        uint8_t mode;
        uint32_t stepTimeUs;
        uint32_t gateTimeUs;
        uint32_t ticksPerUs;
        uint32_t lastStepStartTicks;
        uint8_t tempo;
        bool gate;
        bool playJustPressed;
        uint64_t ledState;
        uint32_t lastKeyState;
        uint8_t seqSyncSource;
        uint8_t savingLed;
        IHardware *hardware;
        void ClearSteps();
        void UpdateLedStates();
        uint8_t GetNoteFromKeyPressed(uint32_t);
        uint8_t GetPatternIndexFromNote(uint8_t);
        void ActivateCurrentStep();
        void OnSavePatternPressed();
        //void OnSelectPatternPressed(uint32_t);
        void OnSeqSyncSelectPressed();
        void OnClearPressed();
        void OnPlayPressed();
        void OnRecordPressed();
        void OnBackPressed();
        void OnNextPressed();
        void OnOctaveDownPressed();
        void OnOctaveUpPressed();
        void OnAccentPressed();
        void OnSlidePressed();
        void OnNoteKeyPressed(uint64_t);
        void OnFunctionKeyReleased();
        void OnKeyPressed(uint32_t);
        void OnKeyReleased(uint32_t);
        void CheckForClockEvent(uint32_t);
        void SavePattern(uint8_t);
        //void LoadPattern(uint8_t);
    };
}

#endif