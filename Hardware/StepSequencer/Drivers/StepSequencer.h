#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#define STEP_SEQUENCER_PROCESS_INTERVAL_US 250

#include "Interface.h"
#include "Controller.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class StepSequencer
    {
    public:
        void Init();
        void SetHardware(IHardware *);
        void Listen();
        bool GetGate();
        uint8_t GetNote();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();
        void SetTempo(uint8_t newTempo);
        Controller controller;

    private:
        Interface interface;
    };
}

#endif