#include "SynthEngine.h"

namespace developmentKit::bassSeed303
{
    void SynthEngine::Init(float prmSampleRate)
    {
        InitOscillator(prmSampleRate);
        InitAdsr(prmSampleRate);
        InitSvf(prmSampleRate);
        sampleRate = prmSampleRate;
    }

    void SynthEngine::Process(float *voiceLeft, float *voiceRight)
    {
        float oscillatorOut, adsrOut, filterOut;
        adsrOut = adsr.Process(gate);
        float accentedLevel = accent ? 0.7 * (1 + accentLevel) : 0.7;
        mainOsc.SetAmp(adsrOut / 5 * accentedLevel * volume);
        mainOsc.SetFreq(noteFrequency);
        oscillatorOut = mainOsc.Process();
        float accentedEnvelopeModulation = accent ? envelopeModulation * (1 + (accentLevel * 2.5)) : envelopeModulation;
        svf.SetFreq(cutOffFrequency * (adsrOut * accentedEnvelopeModulation));
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

        if (accent)
        {
            adsr.SetTime(ADSR_SEG_DECAY, decay * 0.5);
            svf.SetRes(resonance * 1.3);
        }
        else
        {
            adsr.SetTime(ADSR_SEG_DECAY, decay);
            svf.SetRes(resonance);
        }
    }

    void SynthEngine::SetVolume(float newVolume)
    {
        volume = newVolume;
    }

    void SynthEngine::SetCutOffFreq(float newCutOffFrequency)
    {
        cutOffFrequency = Scale(SYNTH_ENGINE_CUTOFF_MIN, sampleRate / 3, newCutOffFrequency);
    }

    void SynthEngine::SetResonance(float newResonance)
    {
        resonance = Scale(0, SYNTH_ENGINE_RESONANCE_MAX, newResonance);
        svf.SetRes(resonance);
    }

    void SynthEngine::SetEnvelopeModulation(float newEnvelopeModulation)
    {
        envelopeModulation = Scale(SYNTH_ENGINE_ENVELOPE_MODULATION_MIN, SYNTH_ENGINE_ENVELOPE_MODULATION_MAX, newEnvelopeModulation);
    }

    void SynthEngine::SetDecay(float newDecay)
    {
        decay = Scale(SYNTH_ENGINE_DECAY_MIN, SYNTH_ENGINE_DECAY_MAX, newDecay);
        adsr.SetTime(ADSR_SEG_DECAY, decay);
    }

    void SynthEngine::SetAccentLevel(float newAccentLevel)
    {
        accentLevel = newAccentLevel;
    }

    void SynthEngine::SetWaveform(uint8_t waveform)
    {
        mainOsc.SetWaveform(waveform);
    }

    float SynthEngine::Scale(float min, float max, float value)
    {
        return (value * (max - min)) + min;
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
        adsr.SetTime(ADSR_SEG_ATTACK, 0);
        adsr.SetTime(ADSR_SEG_DECAY, .1);
        adsr.SetTime(ADSR_SEG_RELEASE, .03);
        adsr.SetSustainLevel(.4);
    }

    void SynthEngine::InitSvf(float sampleRate)
    {
        maxCutoffFrequency = sampleRate / 3;
        svf.Init(sampleRate);
        svf.SetFreq(maxCutoffFrequency);
        svf.SetRes(0);
    }
}