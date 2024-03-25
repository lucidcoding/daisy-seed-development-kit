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
        switch (keyState)
        {
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
            break;
        }
    }

    void SaveState::OnKeyReleased(uint32_t, uint32_t)
    {
    }

    void SaveState::OnNoteKeyPressed(uint64_t keyState)
    {
        uint8_t note = controller->GetNoteFromKeyPressed(keyState);
        uint8_t patternIndex = controller->GetPatternIndexFromNote(note);
        controller->SavePattern(patternIndex);
        controller->Blink((uint64_t)1 << note);
    }
}