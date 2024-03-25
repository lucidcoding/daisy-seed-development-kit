#include "StepRecState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void StepRecState::Reset()
    {

    }

    uint64_t StepRecState::GetLedState()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        uint64_t ledState = (uint64_t)0;
        ledState |= (uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC;
        ledState |= stepIndicator.GetLedState(steps, currentStepIndex);
        return ledState;
    }

    void StepRecState::CheckForClockEvent(uint32_t currentTicks) 
    {

    }

    void StepRecState::OnKeyPressed(uint32_t keyState)
    {

    }
}