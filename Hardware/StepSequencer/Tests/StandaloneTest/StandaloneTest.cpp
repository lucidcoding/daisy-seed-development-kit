#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/NoteEvent.h"
#include "../../Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;

static DaisySeed hardware;
StepSequencer stepSequencer;
Oscillator mainOsc;
Adsr adsr;
bool gate;
Port port;
float noteFreq;
bool slideOn;
bool accent;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float oscillatorOut, adsrOut, portamentoOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 10 * (accent ? 1 : 0.7));
        portamentoOut = port.Process(noteFreq);
        
        if(slideOn)
        {
            mainOsc.SetFreq(portamentoOut);
        }
        else
        {
            mainOsc.SetFreq(noteFreq);
        }
        
        oscillatorOut = mainOsc.Process();
        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    mainOsc.Init(sampleRate);
    mainOsc.SetWaveform(Oscillator::WAVE_SAW);
    mainOsc.SetAmp(0.5);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .05);
    adsr.SetSustainLevel(.2);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);

    float portamento = 0.08;
    port.Init(sampleRate, portamento);
    port.SetHtime(portamento);

    InitAdsr(sampleRate);
    stepSequencer.Init();
    hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");

    while (1)
    {
        stepSequencer.Listen();
        gate = stepSequencer.GetGate();
        noteFreq = mtof(60);
    }
}
