#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#define STEP_SEQUENCER_PROCESS_INTERVAL_US 250

#include "Interface.h"
#include "Controller.h"

namespace developmentKit::stepSequencer
{
    class StepSequencer
    {
    public:
        void Init();
        void Listen();
        bool GetGate();
        uint8_t GetNote();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();

    private:
        Interface interface;
        Controller controller;
        uint32_t lastProcessTimeUs;
    };
}

#endif