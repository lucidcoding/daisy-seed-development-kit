#include "StopState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void StopState::Reset()
    {

    }

    uint64_t StopState::GetLedState()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        uint64_t ledState = (uint64_t)0;
        ledState |= stepIndicator.GetLedState(steps, currentStepIndex);
        return ledState;
    }

    void StopState::CheckForClockEvent(uint32_t currentTicks) 
    {

    }

    void StopState::OnKeyPressed(uint32_t keyState)
    {

    }
}