#include "Debug.h"
#include "PlayState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void PlayState::Reset()
    {
        playJustPressed = true;
    }

    uint64_t PlayState::GetLedState()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        uint64_t ledState = (uint64_t)0;
        ledState |= (uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY;
        ledState |= stepIndicator.GetLedState(steps, currentStepIndex);
        return ledState;
    }

    void PlayState::CheckForClockEvent(uint32_t currentTicks)
    {
        bool gate = controller->GetGate();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        Step *steps = controller->GetSteps();
        uint32_t stepTimeUs = controller->GetStepTimeUs();
        uint32_t gateTimeUs = controller->GetGateTimeUs();
        uint32_t ticksPerUs = controller->GetTicksPerUs();

        if (playJustPressed)
        {
            playJustPressed = false;
            lastStepStartTicks = currentTicks;
        }

        if (gate && (currentTicks - lastStepStartTicks) >= (gateTimeUs * ticksPerUs))
        {
            if (!steps[currentStepIndex].slide)
            {
                gate = false;
                controller->SetGate(gate);
            }
        }

        if ((currentTicks - lastStepStartTicks) >= (stepTimeUs * ticksPerUs))
        {
            lastStepStartTicks = currentTicks;
            currentStepIndex = (currentStepIndex + 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
            controller->SetCurrentStepIndex(currentStepIndex);
            controller->ActivateCurrentStep();
        }
    }

    void PlayState::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnSavePatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed();
            break;
        }
    }

    void PlayState::OnSavePatternPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    }

    void PlayState::OnPlayPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
        controller->SetGate(false);
    }
}