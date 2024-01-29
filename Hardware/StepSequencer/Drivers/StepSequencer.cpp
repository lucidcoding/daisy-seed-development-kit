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
        controller.Init(System::GetTickFreq() / 1000000);
    }

    void StepSequencer::Listen()
    {
        uint32_t currentTicks = System::GetTick();
        uint32_t keyState = interface.ScanNextKeysColumn(currentTicks);
        controller.SetKeyState(keyState);
        controller.Process(System::GetTick());
        uint64_t ledState = controller.GetLedState();
        interface.ScanNextLedsColumn(ledState, currentTicks);
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