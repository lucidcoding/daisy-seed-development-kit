#pragma once
#ifndef BASS_SEED_303_SYNTH_ENGINE_H
#define BASS_SEED_303_SYNTH_ENGINE_H

#include "daisysp.h"
#include "daisy_seed.h"

#define SYNTH_ENGINE_CUTOFF_MIN 0.3f
#define SYNTH_ENGINE_RESONANCE_MAX 0.88f
#define SYNTH_ENGINE_DECAY_MIN 0.05f
#define SYNTH_ENGINE_DECAY_MAX 0.8f
#define SYNTH_ENGINE_ENVELOPE_MODULATION_MIN 0.2f
#define SYNTH_ENGINE_ENVELOPE_MODULATION_MAX 0.8f
#define SYNTH_ENGINE_TEMPO_MAX 240.0f

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
        void SetResonance(float);
        void SetEnvelopeModulation(float);
        void SetDecay(float);
        void SetAccentLevel(float);
        void SetWaveform(uint8_t);

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
        float sampleRate;
        float Scale(float, float, float);
        void InitOscillator(float);
        void InitAdsr(float);
        void InitSvf(float);
    };
}

#endif