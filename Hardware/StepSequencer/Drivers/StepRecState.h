#pragma once
#ifndef STEP_SEQUENCER_STEP_REC_STATE_H
#define STEP_SEQUENCER_STEP_REC_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"
#include "StepIndicator.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StepRecState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void Process(uint32_t, uint32_t);
        void OnKeyPressed(uint32_t, uint32_t);
       // void OnKeyReleased(uint32_t, uint32_t);
        uint8_t GetStateCode();
        void MoveToStep(uint8_t) {}

    private:
        StepIndicator stepIndicator;
        void OnSavePatternPressed();
        void OnSeqSyncSelectPressed();
        void OnClearPressed();
        void OnRecordPressed();
        void OnPlayPressed(uint32_t);
        void OnBackPressed();
        void OnNextPressed();
        void OnOctaveDownPressed();
        void OnOctaveUpPressed();
        void OnAccentPressed();
        void OnSlidePressed();
        void OnNoteKeyPressed(uint64_t);
    };
}

#endif