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
        void SetNoteFrequency(float);
        void SetSlide(bool);
        void SetAccent(bool);
        void SetVolume(float);
        void SetCutOffFreq(float);
        void setResonance(float);
        void setEnvelopeModulation(float);
        void setDecay(float);
        void setAccentLevel(float);

    private:
        Oscillator mainOsc;
        Adsr adsr;
        Svf svf;
        float maxCutoffFrequency;
        bool gate;
        float noteFrequency;
        bool slide;
        bool accent;
        float volume;
        float cutOffFrequency;
        float resonance;
        float envelopeModulation;
        float decay;
        float accentLevel;
        void InitOscillator(float);
        void InitAdsr(float);
        void InitSvf(float);
    };
}

#endif