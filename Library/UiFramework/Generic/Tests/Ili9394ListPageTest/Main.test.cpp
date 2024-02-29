#include "daisysp.h"
#include "daisy_seed.h"
#include "UserInterface.h"
#include "../../Presenters/Root.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9
#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree::view;

static DaisySeed hardware;
Encoder encoder;
Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
UserInterface userInterface;
UiDriver tftDisplay;

void UpdateDisplay()
{
    userInterface.Paint();
}

void ProcessEncoder()
{
    encoder.Debounce();
    int encoderIncrement = encoder.Increment();

    if (encoderIncrement == 1)
    {
        userInterface.Increment();
        UpdateDisplay();
    }

    if (encoderIncrement == -1)
    {
        userInterface.Decrement();
        UpdateDisplay();
    }

    if (encoder.RisingEdge())
    {
        userInterface.Select();
        UpdateDisplay();
    }
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    ProcessEncoder();
    ParameterSet parameterSet = userInterface.GetParameters();
    float level = parameterSet.level;
    float frequency = mtof(parameterSet.note);
    float attackTime = parameterSet.attack;
    float decayTime = parameterSet.decay;
    float sustainLevel = parameterSet.sustain;
    float releaseTime = parameterSet.release;
    uint8_t waveform = parameterSet.waveform;
    adsr.SetTime(ADSR_SEG_ATTACK, attackTime);
    adsr.SetTime(ADSR_SEG_DECAY, decayTime);
    adsr.SetSustainLevel(sustainLevel);
    adsr.SetTime(ADSR_SEG_RELEASE, releaseTime);
    oscillator.SetWaveform(waveform);
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        if (metro.Process())
        {
            hardware.Print("gate");
            gate = !gate;
        }

        adsrOut = adsr.Process(gate);
        oscillator.SetFreq(frequency);
        oscillator.SetAmp(adsrOut * level);
        oscillatorOut = oscillator.Process();

        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    oscillator.Init(sampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_SQUARE);
    oscillator.SetAmp(0.125f);
    oscillator.SetFreq(mtof(64));
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, 0.0f);
    adsr.SetTime(ADSR_SEG_DECAY, 0.125f);
    adsr.SetTime(ADSR_SEG_RELEASE, 0.125f);
    adsr.SetSustainLevel(.01f);
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitEncoder(float sampleRate)
{
    encoder.Init(
        hardware.GetPin(PIN_ENC_A),
        hardware.GetPin(PIN_ENC_B),
        hardware.GetPin(PIN_ENC_CLICK),
        sampleRate);
}

void InitDisplay()
{
    userInterface.Init(&tftDisplay);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitMetro(sampleRate);
    InitEncoder(sampleRate);
    InitDisplay();
    hardware.StartAudio(AudioCallback);
    UpdateDisplay();
    uint32_t lastTicksRefresh = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;

            ParameterSet parameterSet = userInterface.GetParameters();
            hardware.PrintLine("Level: %3.5f, note: %d", parameterSet.level, parameterSet.note);
            tftDisplay.Update();
        }
    }
}