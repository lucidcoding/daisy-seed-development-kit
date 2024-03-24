#include "SaveState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint64_t SaveState::GetLedState(Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t currentStepIndex)
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