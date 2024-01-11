#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H

#include "Keys.h"
#include "Leds.h"
#include "NoteEvent.h"
#include "SequencerBrain.h"

namespace developmentKit::stepSequencer
{
    class StepSequencer
    {
    public:
        void Init();
        void Listen();
        bool GetGate();

    private:
        Keys keys;
        Leds leds;
        SequencerBrain sequencerBrain;
        uint32_t lastProcessTimeUs;
    };
}

#endif