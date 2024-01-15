#include "SynthEngine.h"

namespace developmentKit::bassSeed303
{
    void SynthEngine::Init(float sampleRate)
    {
        InitOscillator(sampleRate);
        InitPort(sampleRate);
        InitAdsr(sampleRate);
    }

    void SynthEngine::Process(float *voiceLeft, float *voiceRight)
    {
        float oscillatorOut, adsrOut, portamentoOut;
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 10 * (accent ? 1 : 0.7));
        portamentoOut = port.Process(noteFreq);

        if(slide)
        {
            mainOsc.SetFreq(portamentoOut);
        }
        else
        {
            mainOsc.SetFreq(noteFreq);
        }
        
        oscillatorOut = mainOsc.Process();
        *voiceLeft = oscillatorOut;
        *voiceRight = oscillatorOut;
    }

    void SynthEngine::SetGate(bool newGate)
    {
        gate = newGate;
    }

    void SynthEngine::SetNoteFreq(float newNoteFreq)
    {
        noteFreq = newNoteFreq;
    }

    void SynthEngine::SetSlide(bool newSlide)
    {
        slide = newSlide;
    }

    void SynthEngine::SetAccent(bool newAccent)
    {
        accent = newAccent;
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

    void SynthEngine::InitPort(float sampleRate)
    {
        float portamento = 0.08;
        port.Init(sampleRate, portamento);
        port.SetHtime(portamento);
    }
}