#include "LoadState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void LoadState::Reset()
    {
        patternIndexToLoad = 255;
    }

    uint64_t LoadState::GetLedState()
    {
        if (patternIndexToLoad == 255)
        {
            return (uint64_t)0x1AB5;
        }
        else
        {
            return (uint64_t)1 << controller->GetNoteFromPatternIndex(patternIndexToLoad);
        }
    }

    void LoadState::CheckForClockEvent(uint32_t currentTicks)
    {
    }

    void LoadState::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_E):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_B):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnNoteKeyPressed(keyState);
            break;
        }
    }

    void LoadState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
        if (((lastKeyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN)) > 0) && ((keyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN)) == 0))
        {
            OnPatternKeyReleased();
        }
    }

    void LoadState::OnNoteKeyPressed(uint64_t keyState)
    {
        uint8_t note = controller->GetNoteFromKeyPressed(keyState);
        patternIndexToLoad = controller->GetPatternIndexFromNote(note);
    }

    void LoadState::OnPatternKeyReleased()
    {
        if (patternIndexToLoad != 255)
        {
            controller->LoadPattern(patternIndexToLoad);
        }

        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    }
}