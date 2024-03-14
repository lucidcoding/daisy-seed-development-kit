#include "daisysp.h"
#include "daisy_seed.h"
#include "SynthEngine.h"
#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "../../Hardware/StepSequencer/Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::hardware::potentiometerArray::drivers;
using namespace developmentKit::hardware::stepSequencer::drivers;
using namespace developmentKit::bassSeed303;

static DaisySeed hardware;
StepSequencer stepSequencer;
SynthEngine synthEngine;
PotentiometerArray potentiometerArray;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    synthEngine.SetVolume(potentiometerArray.analogControl[0].GetRawFloat());
    synthEngine.SetCutOffFreq(potentiometerArray.analogControl[1].GetRawFloat());
    synthEngine.SetResonance(potentiometerArray.analogControl[2].GetRawFloat());
    synthEngine.SetEnvelopeModulation(potentiometerArray.analogControl[3].GetRawFloat());
    synthEngine.SetDecay(potentiometerArray.analogControl[4].GetRawFloat());
    synthEngine.SetAccentLevel(potentiometerArray.analogControl[5].GetRawFloat());
    synthEngine.SetWaveform(potentiometerArray.analogControl[6].GetRawFloat() < 0.5f ? Oscillator::WAVE_SAW : Oscillator::WAVE_SQUARE);

    float voiceLeft, voiceRight;

    for (size_t i = 0; i < size; i += 2)
    {
        synthEngine.Process(&voiceLeft, &voiceRight);
        out[i] = voiceLeft;
        out[i + 1] = voiceRight;
    }
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    synthEngine.Init(sampleRate);
    stepSequencer.Init();
    InitPotentiometerArray();
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    
    while (1)
    {
        stepSequencer.SetTempo(potentiometerArray.analogControl[7].GetRawFloat() * 240.0f);
        stepSequencer.Listen();
        synthEngine.SetGate(stepSequencer.GetGate());
        synthEngine.SetNoteFrequency(mtof(stepSequencer.GetNote()));
        synthEngine.SetAccent(stepSequencer.GetAccent());
    }
}
