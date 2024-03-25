#pragma once
#ifndef STEP_SEQUENCER_BLINK_STATE_H
#define STEP_SEQUENCER_BLINK_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class BlinkState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void CheckForClockEvent(uint32_t currentTicks);
        void OnKeyPressed(uint32_t keyState);
        void SetLedsToBlink(uint64_t);
        void SetTicksPerUs(uint32_t);
        void SetBlinkTimeUs(uint32_t);

    private:
        uint32_t ticksPerUs;
        uint64_t ledsToBlink;
        uint32_t blinkTimeUs;
        uint32_t lastBlinkTicks;
        uint8_t blinkCount;
        bool blinkOn;
        bool blinkJustStarted;
    };
}

#endif