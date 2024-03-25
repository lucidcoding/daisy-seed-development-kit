#pragma once
#ifndef STEP_SEQUENCER_SET_SEQ_SYNC_STATE_H
#define STEP_SEQUENCER_SET_SEQ_SYNC_STATE_H

#include "stdint.h"
#include "Constants.h"
#include "IState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class SetSeqSyncState : public IState
    {
    public:
        void Reset();
        uint64_t GetLedState();
        void CheckForClockEvent(uint32_t currentTicks);
        void OnKeyPressed(uint32_t keyState);
        void SetSeqSyncSource(uint8_t newSeqSyncSource);

    private:
        uint8_t seqSyncSource;
    };
}

#endif