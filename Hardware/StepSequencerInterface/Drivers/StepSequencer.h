#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#include "StepSequencerInterface.h"
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
        StepSequencerInterface stepSequencerInterface;
        Controller controller;
        uint32_t lastProcessTimeUs;
    };
}

#endif