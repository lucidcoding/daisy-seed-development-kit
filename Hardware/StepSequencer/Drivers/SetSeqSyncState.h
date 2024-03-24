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
        uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t);
        void SetSeqSyncSource(uint8_t newSeqSyncSource);

    private:
        uint8_t seqSyncSource;
    };
}

#endif