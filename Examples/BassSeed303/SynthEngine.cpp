#include "SynthEngine.h"

namespace developmentKit::bassSeed303
{
    void SynthEngine::Init(float sampleRate)
    {
        InitOscillator(sampleRate);
        InitAdsr(sampleRate);
        InitSvf(sampleRate);
    }

    void SynthEngine::Process(float *voiceLeft, float *voiceRight)
    {
        float oscillatorOut, adsrOut, filterOut;
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 10 * (accent ? accentLevel : 0.7) * volume);
        mainOsc.SetFreq(noteFrequency);
        oscillatorOut = mainOsc.Process();


        svf.SetFreq(cutOffFrequency + (maxCutoffFrequency / 2 * adsrOut * envelopeModulation));
        svf.Process(oscillatorOut);
        filterOut = svf.Low();

        *voiceLeft = filterOut;
        *voiceRight = filterOut;
    }

    void SynthEngine::SetGate(bool newGate)
    {
        gate = newGate;
    }

    void SynthEngine::SetNoteFrequency(float newNoteFrequency)
    {
        noteFrequency = newNoteFrequency;
    }

    void SynthEngine::SetSlide(bool newSlide)
    {
        slide = newSlide;
    }

    void SynthEngine::SetAccent(bool newAccent)
    {
        accent = newAccent;
    }

    void SynthEngine::SetVolume(float newVolume)
    {
        volume = newVolume;
    }

    void SynthEngine::SetCutOffFreq(float newCutOffFrequency)
    {
        cutOffFrequency = newCutOffFrequency;
    }

    void SynthEngine::setResonance(float newResonance)
    {
        resonance = newResonance;
        svf.SetRes(newResonance);
    }

    void SynthEngine::setEnvelopeModulation(float newEnvelopeModulation)
    {
        envelopeModulation = newEnvelopeModulation;
    }

    void SynthEngine::setDecay(float newDecay)
    {
        decay = newDecay;
        adsr.SetTime(ADSR_SEG_DECAY, newDecay);
    }

    void SynthEngine::setAccentLevel(float newAccentLevel)
    {
        accentLevel = newAccentLevel;
    }

    void SynthEngine::InitOscillator(float sampleRate)
    {
        mainOsc.Init(sampleRate);
        mainOsc.SetWaveform(Oscillator::WAVE_SAW);
        mainOsc.SetAmp(0.5);
    }

    void SynthEngine::InitAdsr(float sampleRate)
    {
        adsr.Init(sampleRate);
        adsr.SetTime(ADSR_SEG_ATTACK, .01);
        adsr.SetTime(ADSR_SEG_DECAY, .1);
        adsr.SetTime(ADSR_SEG_RELEASE, .02);
        adsr.SetSustainLevel(.2);
    }

    void SynthEngine::InitSvf(float sampleRate)
    {
        maxCutoffFrequency = sampleRate / 3;
        svf.Init(sampleRate);
        svf.SetFreq(maxCutoffFrequency);
        svf.SetRes(0);
    }
}