#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "Controller.h"
#include "StepSequencer.h"

namespace developmentKit::stepSequencer
{
    void StepSequencer::Init()
    {
        leds.Init();
        keys.Init();
        controller.Init();
    }

    void StepSequencer::Listen()
    {
        uint32_t currentProcessTimeUs = System::GetUs();

        if (currentProcessTimeUs - lastProcessTimeUs > STEP_SEQUENCER_PROCESS_INTERVAL_US)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            uint32_t lastKeyPress = keys.ScanNextColumn(currentProcessTimeUs);
            controller.SetKeyState(lastKeyPress);
            controller.Process();
            uint64_t ledStates = controller.GetLedStates();
            leds.SetLeds(ledStates);
            leds.ScanNextColumn();
        }
    }

    bool StepSequencer::GetGate()
    {
        return controller.GetGate();
    }

    uint8_t StepSequencer::GetNote()
    {
        return controller.GetNote();
    }

    bool StepSequencer::GetAccent()
    {
        return controller.GetAccent();
    }

    bool StepSequencer::GetSlide()
    {
        return controller.GetSlide();
    }

    bool StepSequencer::GetPreviousSlide()
    {
        return controller.GetPreviousSlide();
    }
}