#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"
#include "UserInterface.h"
#include "../../Library/UiFramework/Utilities/ParameterScaler.h"
#include "../../Hardware/NavigationKeypad/Drivers/NavigationKeypad.h"
#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"
#include "vasynth.h"
#include "main.h"

#define PIN_ENC_A 29
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 30

using namespace daisy;
using namespace daisysp;
using namespace developmentKit::hardware::navigationKeypad::drivers;
using namespace developmentKit::hardware::potentiometerArray::drivers;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;
using namespace developmentKit::library::uiFramework::tree::utilities;

float sysSampleRate;
uint8_t gPlay = PLAY_ON;
static DaisySeed hardware;
Encoder encoder;
bool gate;
NavigationKeypad navigationKeypad;
PotentiometerArray potentiometerArray;
UserInterface userInterface;
UiDriver tftDisplay;
VASynth vasynth;
ParameterScaler transposeParam, cutoffParam, attackParam, releaseParam, detuneParam, portamentoParam;
MidiUartHandler midi;

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

void ProcessControls()
{
    ProcessNavigationKeypad();
    ProcessEncoder();
    ProcessPotentiometerArray();
    ParameterSet parameterSet = userInterface.GetParameters();
    userInterface.UpdateSettings(parameterSet);
    vasynth.waveform_ = parameterSet.osc1WaveShape;
    vasynth.detune_ = detuneParam.Process(parameterSet.osc1Detune);
    vasynth.osc2_waveform_ = parameterSet.osc2WaveShape;
    vasynth.osc2_detune_ = detuneParam.Process(parameterSet.osc2Detune);
    vasynth.voices_ = parameterSet.voices;
    vasynth.portamento_ = portamentoParam.Process(parameterSet.portamento);
    vasynth.osc2_transpose_ = parameterSet.osc2Transpose;
    vasynth.osc2_level_ = parameterSet.osc2Level;
    vasynth.eg_p_attack_ = attackParam.Process(parameterSet.pitchAttack);
    vasynth.eg_p_decay_ = parameterSet.pitchDecay;
    vasynth.eg_p_sustain_ = parameterSet.pitchSustain;
    vasynth.eg_p_release_ = releaseParam.Process(parameterSet.pitchRelease);
    vasynth.noise_level_ = parameterSet.noiseLevel;
    vasynth.eg_f_attack_ = attackParam.Process(parameterSet.filterAttack);
    vasynth.eg_f_decay_ = parameterSet.filterDecay;
    vasynth.eg_f_sustain_ = parameterSet.filterSustain;
    vasynth.eg_f_release_ = releaseParam.Process(parameterSet.filterRelease);
    vasynth.filter_cutoff_ = cutoffParam.Process(parameterSet.cutOff);
    vasynth.filter_res_ = parameterSet.resonance;
    vasynth.filter_type_ = parameterSet.filterType;
    vasynth.eg_f_amount_ = parameterSet.filterEnvelopeAmount;
    vasynth.eg_a_attack_ = attackParam.Process(parameterSet.ampAttack);
    vasynth.eg_a_decay_ = parameterSet.ampDecay;
    vasynth.eg_a_sustain_ = parameterSet.ampSustain;
    vasynth.eg_a_release_ = releaseParam.Process(parameterSet.ampRelease);
    vasynth.pan_ = parameterSet.noiseLevel;
    vasynth.level_ = parameterSet.mixLevel;
    vasynth.lfo_amp_ = parameterSet.lfoLevel;
    vasynth.lfo_freq_ = parameterSet.lfoFrequency;
    vasynth.lfo_waveform_ = parameterSet.lfoWaveShape;
    vasynth.lfo_target_ = parameterSet.lfoTarget;
    vasynth.reverb_level_ = parameterSet.reverbAmount;
    vasynth.reverb_feedback_ = parameterSet.reverbFeedback;
    vasynth.reverb_lpffreq_ = cutoffParam.Process(parameterSet.reverbFilter);
    vasynth.reverb_dry_ = parameterSet.reverbDry;
    vasynth.reverb_wet_ = parameterSet.reverbWet;
    vasynth.delay_level_ = parameterSet.delayAmount;
    vasynth.delay_feedback_ = parameterSet.delayFeedback;
    vasynth.delay_delay_ = parameterSet.delayTime;
    vasynth.SetWaveform();
    vasynth.SetFilter();
    vasynth.SetEG();
    vasynth.SetLFO();
    vasynth.SetReverb();
    vasynth.SetDelay();
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float voice_left, voice_right;
    ProcessControls();

    for (size_t n = 0; n < size; n += 2)
    {
        if (gPlay == PLAY_ON)
        {
            // voices

            vasynth.Process(&voice_left, &voice_right);

            if (vasynth.input_channel_ == INPUT_CHANNEL_NONE)
            {
                out[n] = voice_left;
                out[n + 1] = voice_right;
            }
            else
            {
                out[n] = voice_left + in[n];
                out[n + 1] = voice_right + in[n + 1];
            }
        }
        else
        {
            out[n] = 0;
            out[n + 1] = 0;
        }
    }
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

void InitParams()
{
    cutoffParam.Init(30, 30000, cutoffParam.LOGARITHMIC);
    attackParam.Init(0, 20, attackParam.LOGARITHMIC);
    releaseParam.Init(0, 5, releaseParam.LINEAR);
    detuneParam.Init(0.75, 1, detuneParam.LINEAR);
    portamentoParam.Init(0, 1, portamentoParam.LOGARITHMIC);
}


void InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}

// midi handler
void HandleMidiMessage(MidiEvent m)
{
    switch (m.type)
    {
    case NoteOn:
    {
        NoteOnEvent p = m.AsNoteOn();
        if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
        {
            vasynth.NoteOn(p.note, p.velocity);
        }
        break;
    }
    case NoteOff:
    {
        NoteOnEvent p = m.AsNoteOn();
        if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
        {
            vasynth.NoteOff(p.note);
        }
        break;
    }
    case ControlChange:
    {
        ControlChangeEvent p = m.AsControlChange();
        switch (p.control_number)
        {
        case 74: // cutoff, 0-127 -> frequency
            vasynth.filter_cutoff_ = ((float)p.value / 127.0f) * FILTER_CUTOFF_MAX;
            vasynth.SetFilter();
            break;
        case 71: // res, 0-127
            vasynth.filter_res_ = ((float)p.value / 127.0f);
            vasynth.SetFilter();
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    sysSampleRate = hardware.AudioSampleRate();
    InitNavigationKeypad();
    InitPotentiometerArray();
    InitEncoder(sysSampleRate);
    InitDisplay();
    InitParams();
    InitMidi();
    vasynth.First();
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    UpdateDisplay();
    uint32_t lastTicksRefresh = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    while (1)
    {
        midi.Listen();

        while (midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }

        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            UpdateDisplay();
        }
    }
}