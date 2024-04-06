#pragma once
#ifndef STEP_SEQUENCER_LOAD_STATE_H
#define STEP_SEQUENCER_LOAD_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class LoadState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void Process(uint32_t, uint32_t);
        void OnKeyPressed(uint32_t, uint32_t);
        void OnKeyReleased(uint32_t, uint32_t, uint32_t);
        uint8_t GetStateCode();
        void SetBackgroundState(IState *newBackGroundState);
        void StartPattern();

    private:
        uint32_t lastKeyState;
        uint8_t patternIndexToLoad;
        IState *backgroundState;
        bool loadOnNextBarStart;
        void OnNoteKeyPressed(uint64_t);
        void OnPatternKeyReleased(uint32_t);
    };
}

#endif