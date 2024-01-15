#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#include "Keys.h"
#include "Leds.h"
#include "SequencerBrain.h"

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
        Keys keys;
        Leds leds;
        SequencerBrain sequencerBrain;
        uint32_t lastProcessTimeUs;
    };
}

#endif