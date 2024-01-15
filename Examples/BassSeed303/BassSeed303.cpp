#include "daisysp.h"
#include "daisy_seed.h"
#include "SynthEngine.h"
#include "../../Hardware/StepSequencer/Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;
using namespace developmentKit::bassSeed303;

static DaisySeed hardware;
StepSequencer stepSequencer;
SynthEngine synthEngine;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float voiceLeft, voiceRight;

    for (size_t i = 0; i < size; i += 2)
    {
        synthEngine.Process(&voiceLeft, &voiceRight);
        out[i] = voiceLeft;
        out[i + 1] = voiceRight;
    }
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    synthEngine.Init(sampleRate);
    stepSequencer.Init();
    hardware.StartAudio(AudioCallback);

    while (1)
    {
        stepSequencer.Listen();
        bool gate = stepSequencer.GetGate();
        float noteFreq = mtof(stepSequencer.GetNote());
        bool slide = stepSequencer.GetPreviousSlide();
        bool accent = stepSequencer.GetAccent();
        synthEngine.SetGate(gate);
        synthEngine.SetNoteFreq(noteFreq);
        synthEngine.SetSlide(slide);
        synthEngine.SetAccent(accent);
    }
}
