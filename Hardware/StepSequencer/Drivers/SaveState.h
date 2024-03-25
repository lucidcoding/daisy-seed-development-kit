#pragma once
#ifndef STEP_SEQUENCER_SAVE_STATE_H
#define STEP_SEQUENCER_SAVE_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class SaveState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void CheckForClockEvent(uint32_t);
        void OnKeyPressed(uint32_t);
        void OnKeyReleased(uint32_t, uint32_t);
        void OnNoteKeyPressed(uint64_t);

    private:
    };
}

#endif