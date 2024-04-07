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

    void StopState::Process(uint32_t currentTicks, uint32_t keyState)
    {
        if (keyState != STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            OnKeyPressed(currentTicks, keyState);
        }
    }

    void StopState::OnKeyPressed(uint32_t currentTicks, uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnSavePatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnLoadPatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnClearPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC):
            OnRecordPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed(currentTicks);
            break;
        }
    }

    /*void StopState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
    }*/

    uint8_t StopState::GetStateCode()
    {
        return STEP_SEQUENCER_CONTROLLER_STATE_STOP;
    }

    void StopState::OnSavePatternPressed()
    {
        controller->SwitchToSaveState();
    }

    void StopState::OnSeqSyncSelectPressed()
    {
        controller->SwitchToSetSeqSyncState();
    }

    void StopState::OnLoadPatternPressed()
    {
        controller->SwitchToLoadState(this);
    }

    void StopState::OnClearPressed()
    {
        controller->ClearSteps();
        controller->SwitchToBlinkState(0x1FFFF);
    }

    void StopState::OnRecordPressed()
    {
        controller->SwitchToStepRecState();
    }

    void StopState::OnPlayPressed(uint32_t currentTicks)
    {
        controller->SwitchToPlayStateAndRestart(currentTicks);
    }
}