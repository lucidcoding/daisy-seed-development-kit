#include "daisysp.h"
#include "daisy_seed.h"
#include "DaisyAdapter.h"
#include "SynthEngine.h"
#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "../../Hardware/StepSequencer/Drivers/Debug.h"
#include "../../Hardware/StepSequencer/Drivers/StepSequencer.h"
#include "../../Hardware/Sync/Drivers/Sync.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::hardware::potentiometerArray::drivers;
using namespace developmentKit::hardware::stepSequencer::drivers;
using namespace developmentKit::hardware::sync::drivers;
using namespace developmentKit::bassSeed303;

DaisySeed daisySeed;
StepSequencer stepSequencer;
SynthEngine synthEngine;
DaisyAdapter daisyAdapter;
PotentiometerArray potentiometerArray;
Sync synch;

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
    potentiometerArray.seed = &daisySeed;
    potentiometerArray.Init();
}

int main(void)
{
    daisySeed.Configure();
    daisySeed.Init();
    daisySeed.StartLog(false);
    float sampleRate = daisySeed.AudioSampleRate();
    synch.Init(&daisySeed);
    synthEngine.Init(sampleRate);
    stepSequencer.Init();
    stepSequencer.SetHardware(&daisyAdapter);
    // stepSequencer.controller.daisy = &hardware;
    InitPotentiometerArray();
    daisySeed.adc.Start();
    daisySeed.StartAudio(AudioCallback);

    while (1)
    {
        stepSequencer.SetTempo(potentiometerArray.analogControl[7].GetRawFloat() * 240.0f);
        if (synch.Poll()) stepSequencer.SyncPulse2ppqn();
        stepSequencer.Listen();
        synthEngine.SetGate(stepSequencer.GetGate());
        synthEngine.SetNoteFrequency(mtof(stepSequencer.GetNote()));
        synthEngine.SetAccent(stepSequencer.GetAccent());
    }
}
