#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/Keys.h"
#include "../../Drivers/Leds.h"
#include "../../Drivers/Step.h"
#include "../../Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;

static DaisySeed hardware;
Keys keys;
Leds leds;
StepSequencer stepSequencer;
uint32_t lastProcessTimeUs;
Oscillator mainOsc;
Adsr adsr;
bool gate;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 10);
        oscillatorOut = mainOsc.Process();
        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    mainOsc.Init(sampleRate);
    mainOsc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
    mainOsc.SetAmp(0.5);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.5);
}

void HandleStepMessage(Step step)
{
    gate = true;
    mainOsc.SetFreq(mtof(64 + step.note));
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    leds.Init();
    keys.Init();
    stepSequencer.Init();
    hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");

    while (1)
    {
        uint32_t currentProcessTimeUs = System::GetUs();

        if (currentProcessTimeUs - lastProcessTimeUs > STEP_SEQUENCER_PROCESS_INTERVAL_US)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            uint8_t lastKeyPress = keys.ScanNextColumn(currentProcessTimeUs);
            stepSequencer.SetKeys(lastKeyPress);
            stepSequencer.Process(currentProcessTimeUs);
            uint64_t ledStates = stepSequencer.GetLedStates();
            leds.SetLeds(ledStates);
            leds.ScanNextColumn();

            if (stepSequencer.HasStepEvent())
            {
                Step step = stepSequencer.GetCurrentStep();
                hardware.PrintLine("Note: %d, oct-:%d, oct+:%d, acc:%d, sli:%d", step.note, step.octaveDown, step.octaveUp, step.accent, step.slide);
                HandleStepMessage(step);
            }
        }
    }
}
