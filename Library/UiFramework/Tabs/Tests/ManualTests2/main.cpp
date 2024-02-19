#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "UserInterface.h"
#include "../../Utilities/UiParameter.h"
#include "../../../../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9
#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::utilities;
using namespace developmentKit::library::uiFramework::tabs::view;
using namespace developmentKit::hardware::potentiometerArray::drivers;

static DaisySeed hardware;
Encoder encoder;
static Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
PotentiometerArray potentiometerArray;
UserInterface userInterface;

UiParameter
    levelParameter,
    noteParameter,
    attackParameter,
    decayParameter,
    sustainParameter,
    releaseParameter;

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
        //display.Increment();
    }

    if (encoderIncrement == -1)
    {
        //display.Decrement();
    }

    if (encoder.RisingEdge())
    {
        //display.Select();
    }
}

void ProcessPotentiometerArray()
{
    potentiometerArray.Process();
    float values[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        values[i] = potentiometerArray.analogControl[i].GetRawFloat();
    }

    //display.SetPotentiometerValues(values);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    ProcessEncoder();
    ProcessPotentiometerArray();
    /*float level = levelParameter.Process();
    float frequency = mtof(noteParameter.Process());
    float attackTime = attackParameter.Process();
    float decayTime = decayParameter.Process();
    float sustainLevel = sustainParameter.Process();
    float releaseTime = releaseParameter.Process();
    unsigned int waveform = waveformSettingsPageItem->GetValue();
    adsr.SetTime(ADSR_SEG_ATTACK, attackTime);
    adsr.SetTime(ADSR_SEG_DECAY, decayTime);
    adsr.SetSustainLevel(sustainLevel);
    adsr.SetTime(ADSR_SEG_RELEASE, releaseTime);
    oscillator.SetWaveform(waveform);*/
    float frequency = 220;
    float level = 0.5;
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        if (metro.Process())
        {
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
    adsr.SetSustainLevel(.01);
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
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
    userInterface.Init();
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitPotentiometerArray();
    InitMetro(sampleRate);
    InitEncoder(sampleRate);
    InitDisplay();
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);

    UpdateDisplay();

    uint32_t lastTicksRefresh = System::GetTick();
    uint32_t lastTicksRefreshDisplay = System::GetTick();
    uint32_t lastTicksRShowValues = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            UpdateDisplay();
        }

        if (currentTicks - lastTicksRShowValues > (1000000 * ticksPerUs))
        {
            lastTicksRShowValues = currentTicks;
            hardware.PrintLine("P0: %3.5f\tP1: %3.5f\tP2: %3.5f\tP3: %3.5f\t",
                               potentiometerArray.analogControl[0].GetRawFloat(),
                               potentiometerArray.analogControl[1].GetRawFloat(),
                               potentiometerArray.analogControl[2].GetRawFloat(),
                               potentiometerArray.analogControl[3].GetRawFloat());
        }
    }
}