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
        void CheckForClockEvent(uint32_t);
        void OnKeyPressed(uint32_t);
        void OnKeyReleased(uint32_t, uint32_t);
        void SetSeqSyncSource(uint8_t);
        void OnFunctionKeyReleased();

    private:
        uint8_t seqSyncSource;
        void OnSeqSyncSelectPressed();
    };
}

#endif