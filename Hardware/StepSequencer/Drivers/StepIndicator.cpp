#include "StepIndicator.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint64_t StepIndicator::GetLedState(Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t currentStepIndex)
    {
        Step step = steps[currentStepIndex];
        bool ledStates[STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS];

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS; ledIndex++)
        {
            ledStates[ledIndex] = false;
        }

        for (uint8_t ledToSet = STEP_SEQUENCER_CONTROLLER_LEDS_C; ledToSet <= STEP_SEQUENCER_CONTROLLER_LEDS_C2; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == step.note) && step.gate;
        }

        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP] = step.octaveUp;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT] = step.accent;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE] = step.slide;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_BACK] = false;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_NEXT] = false;

        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + stepIndex] = (stepIndex == currentStepIndex);
        }

        uint64_t ledState = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS; ledIndex++)
        {
            ledState = ledState | ((ledStates[ledIndex] ? (uint64_t)1 : (uint64_t)0) << ledIndex);
        }

        return ledState;
    }
}