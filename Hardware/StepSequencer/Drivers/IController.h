#pragma once
#ifndef STEP_SEQUENCER_ICONTROLLER_H
#define STEP_SEQUENCER_ICONTROLLER_H

#include "stdint.h"
#include "Constants.h"
#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class IController
    {
    public:
        virtual void SetState(uint8_t) = 0;
        virtual void ActivateCurrentStep() = 0;
        virtual bool GetGate() = 0;
        virtual uint8_t GetCurrentStepIndex() = 0;
        virtual Step *GetSteps() = 0;
        virtual uint32_t GetStepTimeUs() = 0;
        virtual uint32_t GetGateTimeUs() = 0;
        virtual uint32_t GetTicksPerUs() = 0;
        //virtual void SetCurrentStepIndex(uint8_t) = 0;
        virtual void SetGate(bool) = 0;
        virtual uint8_t GetNoteFromKeyPressed(uint32_t) = 0;
        virtual uint8_t GetPatternIndexFromNote(uint8_t) = 0;
        virtual void ToggleSeqSyncSource() = 0;
        virtual void Blink(uint64_t) = 0;
        virtual void ClearSteps() = 0;
        virtual void SavePattern(uint8_t) = 0;
        virtual void MoveBackStep() = 0;
        virtual void MoveNextStep() = 0;
        virtual void MoveToFirstStep() = 0;

    private:
    };
}

#endif