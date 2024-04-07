#include "SaveState.h"
#include "Debug.h"
#include "Utilities.h"

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

    void SaveState::Process(uint32_t currentTicks, uint32_t keyState)
    {
        if (keyState != STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            OnKeyPressed(keyState);
        }
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

    uint8_t SaveState::GetStateCode()
    {
        return STEP_SEQUENCER_CONTROLLER_STATE_SAVE;
    }

    void SaveState::OnNoteKeyPressed(uint64_t keyState)
    {
        uint8_t note = Utilities::GetNoteFromKeyPressed(keyState);
        uint8_t patternIndex = Utilities::GetPatternIndexFromNote(note);
        controller->SavePattern(patternIndex);
        controller->SwitchToBlinkState((uint64_t)1 << note);
    }
}