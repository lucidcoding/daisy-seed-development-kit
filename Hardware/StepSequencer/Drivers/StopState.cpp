#include "StopState.h"
#include "Controller.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint64_t StopState::GetLedState(Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t currentStepIndex)
    {
        uint64_t ledState = (uint64_t)0;
        ledState |= stepIndicator.GetLedState(steps, currentStepIndex);
        return ledState;
    }
}