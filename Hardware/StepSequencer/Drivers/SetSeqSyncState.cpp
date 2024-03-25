#include "SetSeqSyncState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void SetSeqSyncState::Reset()
    {
    }

    uint64_t SetSeqSyncState::GetLedState()
    {
        uint64_t ledState = (uint64_t)0;

        switch (seqSyncSource)
        {
        case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL:
            ledState |= (uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1;
            break;
        case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE:
            ledState |= (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 1);
            break;
        case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SYNC:
            ledState |= (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 2);
            break;
        case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SEQ:
            ledState |= (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 3);
            break;
        default:
            ledState |= (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
        };

        return ledState;
    }

    void SetSeqSyncState::CheckForClockEvent(uint32_t currentTicks)
    {
    }

    void SetSeqSyncState::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        }
    }

    void SetSeqSyncState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
        if (((lastKeyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) > 0) && ((keyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) == 0))
        {
            OnFunctionKeyReleased();
        }
    }

    void SetSeqSyncState::OnSeqSyncSelectPressed()
    {
        controller->ToggleSeqSyncSource();
    }

    void SetSeqSyncState::SetSeqSyncSource(uint8_t newSeqSyncSource)
    {
        seqSyncSource = newSeqSyncSource;
    }

    void SetSeqSyncState::OnFunctionKeyReleased()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    }
}