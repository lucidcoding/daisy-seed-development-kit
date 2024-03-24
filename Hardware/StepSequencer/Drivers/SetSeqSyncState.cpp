#include "SetSeqSyncState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint64_t SetSeqSyncState::GetLedState(Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t currentStepIndex)
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

    void SetSeqSyncState::SetSeqSyncSource(uint8_t newSeqSyncSource)
    {
        seqSyncSource = newSeqSyncSource;
    }
}