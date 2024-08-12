
#include "PolySynthEngine.h"

namespace developmentKit::anySynthPolyVa
{
    void PolySynthEngine::Init(float prmSampleRate)
    {
        InitOscillator(prmSampleRate);
        InitAdsr(prmSampleRate);
        InitSvf(prmSampleRate);
        sampleRate = prmSampleRate;

        
        adsr.SetTime(ADSR_SEG_ATTACK, 0.1f);
        adsr.SetTime(ADSR_SEG_DECAY, 0.5f);
        adsr.SetSustainLevel(0.5f);
        adsr.SetTime(ADSR_SEG_RELEASE, 0.5f);
        mainOsc.SetWaveform(Oscillator::WAVE_SAW);
        mainOsc.SetFreq(440);
    }

    void PolySynthEngine::Process(float *voiceLeft, float *voiceRight)
    {
        /*float oscillatorOut, adsrOut, filterOut;
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut);
        oscillatorOut = mainOsc.Process();
        svf.SetFreq(cutOffFrequency);
        svf.Process(oscillatorOut);
        filterOut = svf.Low();


        *voiceLeft = filterOut;
        *voiceRight = filterOut;

        return;*/


        float oscillatorOut, adsrOut, filterOut;
        adsrOut = adsr.Process(gate);
        float accentedLevel = accent ? 0.7 * (1 + accentLevel) : 0.7;
        mainOsc.SetAmp(adsrOut / 5 * accentedLevel * volume);
        //mainOsc.SetFreq(noteFrequency);
        oscillatorOut = mainOsc.Process();
        float accentedEnvelopeModulation = accent ? envelopeModulation * (1 + (accentLevel * 2.5)) : envelopeModulation;
        svf.SetFreq(cutOffFrequency * (adsrOut * accentedEnvelopeModulation));
        svf.Process(oscillatorOut);
        filterOut = svf.Low();

        *voiceLeft = filterOut;
        *voiceRight = filterOut;
    }

    void PolySynthEngine::SetGate(bool newGate)
    {
        gate = newGate;
    }

    void PolySynthEngine::SetNoteFrequency(float newNoteFrequency)
    {
        noteFrequency = newNoteFrequency;
    }

    void PolySynthEngine::SetSlide(bool newSlide)
    {
        slide = newSlide;
    }

    void PolySynthEngine::SetAccent(bool newAccent)
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

    void PolySynthEngine::SetVolume(float newVolume)
    {
        volume = newVolume;
    }

    void PolySynthEngine::SetCutOffFreq(float newCutOffFrequency)
    {
        cutOffFrequency = Scale(SYNTH_ENGINE_CUTOFF_MIN, sampleRate / 3, newCutOffFrequency);
    }

    void PolySynthEngine::SetResonance(float newResonance)
    {
        resonance = Scale(0, SYNTH_ENGINE_RESONANCE_MAX, newResonance);
        svf.SetRes(resonance);
    }

    void PolySynthEngine::SetEnvelopeModulation(float newEnvelopeModulation)
    {
        envelopeModulation = Scale(SYNTH_ENGINE_ENVELOPE_MODULATION_MIN, SYNTH_ENGINE_ENVELOPE_MODULATION_MAX, newEnvelopeModulation);
    }

    void PolySynthEngine::SetDecay(float newDecay)
    {
        decay = Scale(SYNTH_ENGINE_DECAY_MIN, SYNTH_ENGINE_DECAY_MAX, newDecay);
        adsr.SetTime(ADSR_SEG_DECAY, decay);
    }

    void PolySynthEngine::SetAccentLevel(float newAccentLevel)
    {
        accentLevel = newAccentLevel;
    }

    void PolySynthEngine::SetWaveform(uint8_t waveform)
    {
        mainOsc.SetWaveform(waveform);
    }

    float PolySynthEngine::Scale(float min, float max, float value)
    {
        return (value * (max - min)) + min;
    }

    void PolySynthEngine::InitOscillator(float sampleRate)
    {
        mainOsc.Init(sampleRate);
        mainOsc.SetWaveform(Oscillator::WAVE_SAW);
        mainOsc.SetAmp(0.5);
    }

    void PolySynthEngine::InitAdsr(float sampleRate)
    {
        adsr.Init(sampleRate);
        adsr.SetTime(ADSR_SEG_ATTACK, 0);
        adsr.SetTime(ADSR_SEG_DECAY, .1);
        adsr.SetTime(ADSR_SEG_RELEASE, .03);
        adsr.SetSustainLevel(.4);
    }

    void PolySynthEngine::InitSvf(float sampleRate)
    {
        maxCutoffFrequency = sampleRate / 3;
        svf.Init(sampleRate);
        svf.SetFreq(maxCutoffFrequency);
        svf.SetRes(0);
    }
}
