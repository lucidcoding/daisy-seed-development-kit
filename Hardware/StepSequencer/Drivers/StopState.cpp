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
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnSavePatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnClearPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC):
            OnRecordPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed();
            break;
        }
    }

    void StopState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
    }

    void StopState::OnSavePatternPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    }

    void StopState::OnSeqSyncSelectPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    }

    void StopState::OnClearPressed()
    {
        controller->ClearSteps();
        controller->Blink(0x1FFFF);
    }

    void StopState::OnRecordPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
    }

    void StopState::OnPlayPressed()
    {
        controller->SetCurrentStepIndex(0);
        controller->ActivateCurrentStep();
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
    }
}