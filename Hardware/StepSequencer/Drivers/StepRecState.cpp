#include "StepRecState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void StepRecState::Reset()
    {
        controller->MoveToFirstStep();
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
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_BACK):
            OnBackPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT):
            OnNextPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN):
            OnOctaveDownPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP):
            OnOctaveUpPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT):
            OnAccentPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE):
            OnSlidePressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_E):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_B):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnNoteKeyPressed(keyState);
        }
    }

    void StepRecState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
    }

    void StepRecState::OnSavePatternPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    }

    void StepRecState::OnSeqSyncSelectPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    }

    void StepRecState::OnClearPressed()
    {
        controller->ClearSteps();
        controller->Blink(0x1FFFF);
    }

    void StepRecState::OnPlayPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
    }

    void StepRecState::OnBackPressed()
    {
        controller->MoveBackStep();
    }

    void StepRecState::OnNextPressed()
    {
        controller->MoveNextStep();
    }

    void StepRecState::OnOctaveDownPressed()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        steps[currentStepIndex].octaveDown = !steps[currentStepIndex].octaveDown;
    }

    void StepRecState::OnOctaveUpPressed()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        steps[currentStepIndex].octaveUp = !steps[currentStepIndex].octaveUp;
    }

    void StepRecState::OnAccentPressed()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        steps[currentStepIndex].accent = !steps[currentStepIndex].accent;
    }

    void StepRecState::OnSlidePressed()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        steps[currentStepIndex].slide = !steps[currentStepIndex].slide;
    }

    void StepRecState::OnNoteKeyPressed(uint64_t keyState)
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        uint8_t note = controller->GetNoteFromKeyPressed(keyState);

        if (note != STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY)
        {
            if (steps[currentStepIndex].note == note)
            {
                steps[currentStepIndex].gate = !steps[currentStepIndex].gate;
            }
            else
            {
                steps[currentStepIndex].gate = true;
                steps[currentStepIndex].note = note;
            }
        }
    }
}