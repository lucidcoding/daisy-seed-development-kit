#pragma once
#ifndef STATE_FACTORY_H
#define STATE_FACTORY_H

#include "stdint.h"
#include "IState.h"
#include "BlinkState.h"
#include "LoadState.h"
#include "PlayState.h"
#include "SaveState.h"
#include "SetSeqSyncState.h"
#include "StepRecState.h"
#include "StopState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StateFactory
    {
    public:
        IState *GetBlinkState(uint64_t);
        IState *GetLoadState(IState *);
        IState *GetPlayStateAndRestart(uint32_t);
        IState *GetPlayStateAndContinue();
        IState *GetStopState();
        IState *GetStepRecState();
        IState *GetSaveState();
        IState *GetSetSeqSyncState();

    private:
        BlinkState blinkState;
        StopState stopState;
        LoadState loadState;
        PlayState playState;
        SaveState saveState;
        StepRecState stepRecState;
        SetSeqSyncState setSeqSyncState;
    };
}

#endif