#include "SaveState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void SaveState::Reset()
    {

    }
    uint64_t SaveState::GetLedState()
    {
        uint64_t ledState = (uint64_t)0x1AB5;
        return ledState;
    }

    void SaveState::CheckForClockEvent(uint32_t currentTicks) 
    {

    }

    void SaveState::OnKeyPressed(uint32_t keyState)
    {

    }
}