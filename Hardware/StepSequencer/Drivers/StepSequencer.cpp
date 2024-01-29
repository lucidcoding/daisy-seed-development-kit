#include "stdint.h"
#include "Keys.h"
#include "Leds.h"
#include "Controller.h"
#include "StepSequencer.h"

namespace developmentKit::stepSequencer
{
    void StepSequencer::Init()
    {
        interface.Init();
        controller.Init();
    }

    void StepSequencer::Listen()
    {
        uint32_t currentProcessTimeUs = System::GetUs();
        uint32_t keyState = interface.ScanNextKeysColumn(currentProcessTimeUs);
        controller.SetKeyState(keyState);
        controller.Process(currentProcessTimeUs);
        uint64_t ledState = controller.GetLedState();
        interface.ScanNextLedsColumn(ledState, currentProcessTimeUs);
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

    void StepSequencer::SetTempo(uint8_t newTempo)
    {
        controller.SetTempo(newTempo);
    }
}