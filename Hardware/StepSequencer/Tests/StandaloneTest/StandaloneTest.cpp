#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/Keys.h"
#include "../../Drivers/Leds.h"
#include "../../Drivers/NoteEvent.h"
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
Port port;
float noteFreq;
bool slideOn;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float oscillatorOut, adsrOut, portamentoOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 10);
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
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.2);
}

void HandleStepMessage(NoteEvent noteEvent)
{
    if (noteEvent.type == STEP_SEQUENCER_NOTE_EVENT_TYPE_NOTE_ON)
    {
        hardware.PrintLine("on");
        gate = true;
        noteFreq = mtof(64 + noteEvent.note);

        if(noteEvent.octaveUp)
        {
            noteFreq += 12;
        }

        if(noteEvent.octaveDown)
        {
            noteFreq -= 12;
        }

        slideOn = noteEvent.slide;

        /*if(noteEvent.slide)
        {
            port.SetHtime(0.1);
        }
        else
        {
            port.SetHtime(0);
        }*/
    }
    else if (noteEvent.type == STEP_SEQUENCER_NOTE_EVENT_TYPE_NOTE_OFF)
    {
        hardware.PrintLine("off");
        gate = false;
    }
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);

    //https://github.com/moonfriendsynth/OscPocket-VA-Daisy-Pod/blob/main/vasynth.cpp
    float portamento = 0.08;
    port.Init(sampleRate, portamento);
    port.SetHtime(portamento);

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
                NoteEvent noteEvent = stepSequencer.GetCurrentStep();
                //hardware.PrintLine("type:%d, note: %d, oct-:%d, oct+:%d, acc:%d, sli:%d", noteEvent.type, noteEvent.note, noteEvent.octaveDown, noteEvent.octaveUp, noteEvent.accent, noteEvent.slide);
                HandleStepMessage(noteEvent);
            }
        }
    }
}
