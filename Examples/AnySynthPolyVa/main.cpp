#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"
#include "UserInterface.h"
#include "../../Library/UiFramework/Utilities/ParameterScaler.h"
#include "../../Hardware/NavigationKeypad/Drivers/NavigationKeypad.h"
#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"
#include "PolySynthEngine.h"
#include "main.h"

#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisy;
using namespace daisysp;
using namespace developmentKit::anySynthPolyVa;
using namespace developmentKit::hardware::navigationKeypad::drivers;
using namespace developmentKit::hardware::potentiometerArray::drivers;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;

float sysSampleRate;
// uint8_t gPlay = PLAY_ON;
static DaisySeed hardware;
Encoder encoder;
Metro metro;
bool gate;
NavigationKeypad navigationKeypad;
PotentiometerArray potentiometerArray;
UserInterface userInterface;
UiDriver tftDisplay;
PolySynthEngine synthEngine;

Oscillator oscillator;
Adsr adsr;

void UpdateDisplay()
{
    userInterface.Paint();
    tftDisplay.Update();
}

void ProcessNavigationKeypad()
{
    uint8_t keyState = navigationKeypad.ScanKeys();

    if (keyState == NAVIGATION_KEYPAD_KEY_LEFT)
    {
        userInterface.Left();
        UpdateDisplay();
    }

    if (keyState == NAVIGATION_KEYPAD_KEY_RIGHT)
    {
        userInterface.Right();
        UpdateDisplay();
    }

    if (keyState == NAVIGATION_KEYPAD_KEY_MENU)
    {
        userInterface.Menu();
        UpdateDisplay();
    }

    if (keyState == NAVIGATION_KEYPAD_KEY_ENTER)
    {
        userInterface.Select();
        hardware.PrintLine("select!");
        UpdateDisplay();
    }
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
        hardware.PrintLine("select!");
        UpdateDisplay();
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

    userInterface.SetPotentiometerValues(values);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float voice_left, voice_right;
    ProcessNavigationKeypad();
    ProcessEncoder();
    ProcessPotentiometerArray();
    ParameterSet parameterSet = userInterface.GetParameters();
    userInterface.UpdateSettings(parameterSet);

    synthEngine.SetVolume(parameterSet.noiseLevel);
    synthEngine.SetCutOffFreq(parameterSet.cutOff);
    synthEngine.SetResonance(parameterSet.resonance);
    synthEngine.SetEnvelopeModulation(parameterSet.osc2Level);

    if (metro.Process())
    {
        hardware.Print("gate");
        gate = !gate;
    }

    for (size_t i = 0; i < size; i += 2)
    {

        /*float oscillatorOut, adsrOut;
        adsr.SetTime(ADSR_SEG_ATTACK, 0.1f);
        adsr.SetTime(ADSR_SEG_DECAY, 0.5f);
        adsr.SetSustainLevel(0.5f);
        adsr.SetTime(ADSR_SEG_RELEASE, 0.5f);
        oscillator.SetWaveform(Oscillator::WAVE_SAW);
        adsrOut = adsr.Process(gate);
        oscillator.SetFreq(440);
        oscillator.SetAmp(adsrOut);
        oscillatorOut = oscillator.Process();

        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;*/

        synthEngine.Process(&voice_left, &voice_right);

        out[i] = voice_left;
        out[i + 1] = voice_right;
    }

    return;
    // float frequency = mtof(parameterSet.note);
    /*vasynth.waveform_ = parameterSet.osc1WaveShape;
    vasynth.detune_ = parameterSet.osc1Detune;
    vasynth.osc2_waveform_ = parameterSet.osc2WaveShape;
    vasynth.osc2_transpose_ = parameterSet.osc2Coarse;
    vasynth.osc2_detune_ = parameterSet.osc2Detune;
    vasynth.osc2_level_ = parameterSet.osc2Level;
    vasynth.noise_level_ = parameterSet.noiseLevel;
    vasynth.lfo_amp_ = 0;
    vasynth.lfo_freq_ = 1000;
    //vasynth.osc2_waveform_ = parameterSet.osc2WaveShape;

    //vasynth.noise_ = parameterSet.noiseLevel;

    vasynth.filter_cutoff_ = parameterSet.cutOff * 30000;
    vasynth.filter_res_ = parameterSet.resonance;





    vasynth.SetWaveform();
    vasynth.SetFilter();*/

    synthEngine.SetVolume(parameterSet.noiseLevel);
    synthEngine.SetCutOffFreq(parameterSet.cutOff);
    synthEngine.SetResonance(parameterSet.resonance);
    synthEngine.SetEnvelopeModulation(parameterSet.osc2Level);

    // float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {

        synthEngine.Process(&voice_left, &voice_right);
        out[i] = voice_left;
        out[i + 1] = voice_right;
        /*adsrOut = adsr.Process(gate);
        oscillator.SetFreq(440);
        //oscillator.SetFreq(frequency);
        //oscillator.SetAmp(adsrOut);
        oscillator.SetAmp(adsrOut * level);
        oscillatorOut = oscillator.Process();

        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;*/
    }
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitNavigationKeypad()
{
    navigationKeypad.Init();
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
    userInterface.Init(&tftDisplay);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    sysSampleRate = hardware.AudioSampleRate();
    InitNavigationKeypad();
    InitPotentiometerArray();
    InitMetro(sysSampleRate);
    InitEncoder(sysSampleRate);

    oscillator.Init(sysSampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_SQUARE);
    oscillator.SetAmp(0.125f);
    oscillator.SetFreq(mtof(64));
    adsr.Init(sysSampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, 0.0f);
    adsr.SetTime(ADSR_SEG_DECAY, 0.125f);
    adsr.SetTime(ADSR_SEG_RELEASE, 0.125f);
    adsr.SetSustainLevel(.01f);

    InitDisplay();
    synthEngine.Init(sysSampleRate);
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    UpdateDisplay();
    synthEngine.SetNoteFrequency(mtof(127));

    uint32_t lastTicksRefresh = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    uint16_t noteCountUp = 0;

    while (1)
    {

        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            if (noteCountUp > 10)
            {
                noteCountUp = 0;
                hardware.Print("gate");
                gate = !gate;
            }
            noteCountUp++;
            synthEngine.SetGate(gate);

            lastTicksRefresh = currentTicks;
            hardware.PrintLine("Tick...");
            // hardware.PrintLine("P1: %3.5f, P2: %3.5f", potentiometerArray.analogControl[0].GetRawFloat(), potentiometerArray.analogControl[1].GetRawFloat());
            UpdateDisplay();
        }
    }
}