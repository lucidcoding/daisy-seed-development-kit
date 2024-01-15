#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "SequencerBrain.h"
#include "StepSequencer.h"

namespace developmentKit::stepSequencer
{
    void StepSequencer::Init()
    {
        leds.Init();
        keys.Init();
        sequencerBrain.Init();
    }

    void StepSequencer::Listen()
    {
        uint32_t currentProcessTimeUs = System::GetUs();

        if (currentProcessTimeUs - lastProcessTimeUs > STEP_SEQUENCER_PROCESS_INTERVAL_US)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            uint8_t lastKeyPress = keys.ScanNextColumn(currentProcessTimeUs);
            sequencerBrain.SetLastKeyPress(lastKeyPress);
            sequencerBrain.Process();
            uint64_t ledStates = sequencerBrain.GetLedStates();
            leds.SetLeds(ledStates);
            leds.ScanNextColumn();
        }
    }
    
    bool StepSequencer::GetGate()
    {
        return sequencerBrain.GetGate();
    }

    uint8_t StepSequencer::GetNote()
    {
        return sequencerBrain.GetNote();
    }
}