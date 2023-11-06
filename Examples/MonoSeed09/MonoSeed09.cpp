#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Hardware/PotentiometerArray/PotentiometerArray.h"

#define MIN_PULSE_WIDTH 0.05f
#define MIDI_CHANNEL 0

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
PotentiometerArray potentiometerArray;
MidiUartHandler midi;
Oscillator lfo, mainOsc, subOsc;
Adsr adsr;
Svf svf;
bool gate;
float maxCutoffFrequency;
float midiNoteFreq;

Parameter
    masterVolumeParam,
    lfoShapeParam,
    lfoRateParam,
    mainOscShapeParam,
    mainOscLfoModParam,
    mainOscPulseWidthParam,
    mainOscPulseWidthLfoModParam,
    subOscLevelParam,
    svfCutOffFrequencyParam,
    svfResonanceParam,
    svfAdsrModParam,
    svfLfoModParam,
    attackParam,
    decayParam,
    sustainParam,
    releaseParam;

void SetLfoShape(float lfoShapeValue)
{
    if (lfoShapeValue >= 0 && lfoShapeValue < 1.0f)
    {
        lfo.SetWaveform(Oscillator::WAVE_SIN);
    }
    else if (lfoShapeValue >= 1.0f && lfoShapeValue < 2.0f)
    {
        lfo.SetWaveform(Oscillator::WAVE_TRI);
    }
    else  if (lfoShapeValue >= 2.0f && lfoShapeValue < 3.0f)
    {
        lfo.SetWaveform(Oscillator::WAVE_SAW);
    }
    else  if (lfoShapeValue >= 3.0f && lfoShapeValue < 4.0f)
    {
        lfo.SetWaveform(Oscillator::WAVE_SQUARE);
    }
}

void SetMainOscShape(float mainOscShapeValue)
{
    if (mainOscShapeValue >= 0 && mainOscShapeValue < 1.0f)
    {
        mainOsc.SetWaveform(Oscillator::WAVE_TRI);
    }
    else if (mainOscShapeValue >= 1.0f && mainOscShapeValue < 2.0f)
    {
        mainOsc.SetWaveform(Oscillator::WAVE_SAW);
    }
    else
    {
        mainOsc.SetWaveform(Oscillator::WAVE_SQUARE);
    }
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    float masterVolume = masterVolumeParam.Process();
    float lfoRate = lfoRateParam.Process();
    float lfoShapeValue = lfoShapeParam.Process();
    float mainOscShapeValue = mainOscShapeParam.Process();
    float mainOscLfoMod = mainOscLfoModParam.Process();
    float mainOscPulseWidth = mainOscPulseWidthParam.Process();
    float mainOscPulseWidthLfoMod = mainOscPulseWidthLfoModParam.Process();
    float subOscLevel = subOscLevelParam.Process();
    float svfCutOffFrequency = svfCutOffFrequencyParam.Process();
    float svfResonance = svfResonanceParam.Process();
    float svfAdsrMod = svfAdsrModParam.Process();
    float svfLfoMod = svfLfoModParam.Process();
    float attackTime = attackParam.Process();
    float decayTime = decayParam.Process();
    float sustainLevel = sustainParam.Process();
    float releaseTime = releaseParam.Process();
    lfo.SetFreq(lfoRate);
    SetMainOscShape(mainOscShapeValue);
    SetLfoShape(lfoShapeValue);
    svf.SetRes(svfResonance);
    adsr.SetTime(ADSR_SEG_ATTACK, attackTime);
    adsr.SetTime(ADSR_SEG_DECAY, decayTime);
    adsr.SetSustainLevel(sustainLevel);
    adsr.SetTime(ADSR_SEG_RELEASE, releaseTime);
    float adsrOut, lfoOut, mainOscOut, subOscOut, filterOut;
    const float adsrFactor = 10;

    for (size_t i = 0; i < size; i += 2)
    {
        // Calculate amplitude and frequency of main oscillator and process
        adsrOut = adsr.Process(gate);
        lfo.SetAmp(mainOscLfoMod);
        lfoOut = lfo.Process();
        mainOsc.SetAmp(adsrOut / adsrFactor * masterVolume);
        mainOsc.SetFreq(midiNoteFreq + lfoOut);
        mainOscOut = mainOsc.Process();

        // Calculate amplitude and frequency of sub-oscillator and process
        subOsc.SetAmp(adsrOut / adsrFactor * subOscLevel * masterVolume);
        subOsc.SetFreq((midiNoteFreq / 2) + lfoOut);
        subOscOut = subOsc.Process();

        // Calculate pulse-width of main oscillator and process
        lfo.SetAmp(mainOscPulseWidthLfoMod);
        lfoOut = lfo.Process();
        float finalPulseWidth = mainOscPulseWidth + lfoOut;
        finalPulseWidth = finalPulseWidth > MIN_PULSE_WIDTH ? finalPulseWidth : MIN_PULSE_WIDTH;
        mainOsc.SetPw(finalPulseWidth);

        // Calculate frequency of filter and process
        lfo.SetAmp(svfLfoMod);
        lfoOut = lfo.Process();
        svf.SetFreq(svfCutOffFrequency + (maxCutoffFrequency / 2 * adsrOut * svfAdsrMod) + (maxCutoffFrequency / 2 * lfoOut));
        svf.Process(mainOscOut + subOscOut);
        filterOut = svf.Low();

        // Send output of filter to output
        out[i] = filterOut;
        out[i + 1] = filterOut;
    }
}

void InitLfo(float sampleRate)
{
    lfo.Init(sampleRate);
    lfo.SetWaveform(Oscillator::WAVE_SIN);
    lfo.SetFreq(1000.0f);
    lfo.SetAmp(10.0f);
}

void InitOscillator(float sampleRate)
{
    mainOsc.Init(sampleRate);
    mainOsc.SetWaveform(Oscillator::WAVE_SAW);
    mainOsc.SetAmp(0.1f);
    mainOsc.SetPw(0.5f);
}

void InitSubOsc(float sampleRate)
{
    subOsc.Init(sampleRate);
    subOsc.SetWaveform(Oscillator::WAVE_SQUARE);
    subOsc.SetAmp(0.1f);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.1);
}

void InitSvf(float sampleRate)
{
    maxCutoffFrequency = sampleRate / 3;
    svf.Init(sampleRate);
    svf.SetFreq(maxCutoffFrequency);
    svf.SetRes(0);
}

void InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitParameters(float sampleRate)
{
    masterVolumeParam.Init(potentiometerArray.analogControl[0], 0, 1.0f, Parameter::LINEAR);
    lfoRateParam.Init(potentiometerArray.analogControl[1], 0.25f, 20.0f, Parameter::LOGARITHMIC);
    lfoShapeParam.Init(potentiometerArray.analogControl[2], 0, 4.0f, Parameter::LINEAR);
    mainOscShapeParam.Init(potentiometerArray.analogControl[3], 0, 3.0f, Parameter::LINEAR);
    mainOscLfoModParam.Init(potentiometerArray.analogControl[4], 0, 100.0f, Parameter::LINEAR);
    mainOscPulseWidthParam.Init(potentiometerArray.analogControl[5], MIN_PULSE_WIDTH, 0.51f, Parameter::LINEAR);
    mainOscPulseWidthLfoModParam.Init(potentiometerArray.analogControl[6], 0, 0.5f, Parameter::LINEAR);
    subOscLevelParam.Init(potentiometerArray.analogControl[7], 0, 1.0f, Parameter::LINEAR);
    svfCutOffFrequencyParam.Init(potentiometerArray.analogControl[8], 0, sampleRate / 3, Parameter::LINEAR);
    svfResonanceParam.Init(potentiometerArray.analogControl[9], 0, 1.0f, Parameter::LINEAR);
    svfAdsrModParam.Init(potentiometerArray.analogControl[10], 0, 1.0f, Parameter::LINEAR);
    svfLfoModParam.Init(potentiometerArray.analogControl[11], 0, 1.0f, Parameter::LINEAR);
    attackParam.Init(potentiometerArray.analogControl[12], 0.001f, 1.0f, Parameter::LINEAR);
    decayParam.Init(potentiometerArray.analogControl[13], 0.025f, 1.0f, Parameter::LINEAR);
    sustainParam.Init(potentiometerArray.analogControl[14], 0., 1.0f, Parameter::LINEAR);
    releaseParam.Init(potentiometerArray.analogControl[15], 0, 1.0f, Parameter::LINEAR);
}

void HandleMidiMessage(MidiEvent m)
{
    if (m.channel == MIDI_CHANNEL)
    {
        switch (m.type)
        {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();

            if (m.data[1] != 0)
            {
                p = m.AsNoteOn();
                gate = true;
                midiNoteFreq = mtof(p.note);
            }

            break;
        }
        case NoteOff:
        {
            gate = false;
            break;
        }
        default:
            break;
        }
    }
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    InitLfo(sampleRate);
    InitOscillator(sampleRate);
    InitSubOsc(sampleRate);
    InitAdsr(sampleRate);
    InitSvf(sampleRate);
    InitMidi();
    InitPotentiometerArray();
    InitParameters(sampleRate);
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    midi.StartReceive();

    while (1)
    {
        midi.Listen();

        while (midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
    }
}
