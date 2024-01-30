#include "daisysp.h"
#include "daisy_seed.h"
#include "SynthEngine.h"
#include "../../Hardware/PotentiometerArray/PotentiometerArray.h"
#include "../../Hardware/StepSequencer/Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
//using namespace developmentKit;
using namespace developmentKit::stepSequencer;
using namespace developmentKit::bassSeed303;

static DaisySeed hardware;
StepSequencer stepSequencer;
SynthEngine synthEngine;
developmentKit::PotentiometerArray potentiometerArray;

Parameter masterVolumeParam, cutOffFrequencyParam, resonanceParam, envelopeModulationParam, decayParam, accentLevelParam, tempoParam;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();

    synthEngine.SetVolume(masterVolumeParam.Process());
    synthEngine.SetCutOffFreq(cutOffFrequencyParam.Process());
    synthEngine.setResonance(resonanceParam.Process());
    synthEngine.setEnvelopeModulation(envelopeModulationParam.Process());
    synthEngine.setDecay(decayParam.Process());
    synthEngine.setAccentLevel(accentLevelParam.Process());

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

void InitParameters(float sampleRate)
{
    masterVolumeParam.Init(potentiometerArray.analogControl[0], 0, 1.0f, Parameter::LINEAR);
    cutOffFrequencyParam.Init(potentiometerArray.analogControl[1], 0, sampleRate / 3, Parameter::LINEAR);
    resonanceParam.Init(potentiometerArray.analogControl[2], 0, 1.0f, Parameter::LINEAR);
    decayParam.Init(potentiometerArray.analogControl[3], 0.025f, 1.0f, Parameter::LINEAR);
    envelopeModulationParam.Init(potentiometerArray.analogControl[4], 0, 1.0f, Parameter::LINEAR);
    accentLevelParam.Init(potentiometerArray.analogControl[5], 0, 1.0f, Parameter::LINEAR);
    tempoParam.Init(potentiometerArray.analogControl[6], 0, 240.0f, Parameter::LINEAR);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    synthEngine.Init(sampleRate);
    stepSequencer.Init();
    InitPotentiometerArray();
    InitParameters(sampleRate);
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    
    while (1)
    {
        stepSequencer.SetTempo(tempoParam.Process());
        stepSequencer.Listen();
        synthEngine.SetGate(stepSequencer.GetGate());
        synthEngine.SetNoteFrequency(mtof(stepSequencer.GetNote()));
        synthEngine.SetAccent(stepSequencer.GetAccent());
    }
}
