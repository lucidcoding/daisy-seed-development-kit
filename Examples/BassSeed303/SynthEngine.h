#pragma once
#ifndef BASS_SEED_303_SYNTH_ENGINE_H
#define BASS_SEED_303_SYNTH_ENGINE_H

#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

namespace developmentKit::bassSeed303
{
    class SynthEngine
    {
    public:
        SynthEngine() {}
        ~SynthEngine() {}
        void Init(float);
        void Process(float *, float *);
        void SetGate(bool);
        void SetNoteFreq(float);
        void SetSlide(bool);
        void SetAccent(bool);

    private:
        Oscillator mainOsc;
        Adsr adsr;
        Port port;
        bool gate;
        float noteFreq;
        bool slide;
        bool accent;
        void InitOscillator(float);
        void InitAdsr(float);
        void InitPort(float);
    };
}

#endif