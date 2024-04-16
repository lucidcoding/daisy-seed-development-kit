#include "Debug.h"
#include "StateFactory.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    IState *StateFactory::GetBlinkState(uint64_t ledsToBlink)
    {
        blinkState.SetLedsToBlink(ledsToBlink);
        blinkState.Reset();
        return &blinkState;
    }

    IState *StateFactory::GetLoadState(IState *backgroundState)
    {
        loadState.SetBackgroundState(backgroundState);
        loadState.Reset();
        return &loadState;
    }

    IState *StateFactory::GetPlayStateAndRestart(uint32_t currentTicks)
    {
        playState.Start(currentTicks);
        return &playState;
    }

    IState *StateFactory::GetPlayStateAndContinue()
    {
        return &playState;
    }

    IState *StateFactory::GetStopState()
    {
        stopState.Reset();
        return &stopState;
    }

    IState *StateFactory::GetStepRecState()
    {
        stepRecState.Reset();
        return &stepRecState;
    }

    IState *StateFactory::GetSaveState()
    {
        saveState.Reset();
        return &saveState;
    }

    IState *StateFactory::GetSetSeqSyncState()
    {
        setSeqSyncState.Reset();
        return &setSeqSyncState;
    }
}