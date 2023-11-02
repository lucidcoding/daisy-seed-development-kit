#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Hardware/PotentiometerArray/PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
PotentiometerArray potentiometerArray;
MidiUartHandler midi;
Oscillator lfo;
Oscillator oscillator;
Oscillator subOsc;
Adsr adsr;
Svf svf;
bool gate;
int midiChannel;
float maxCutoffFrequency;
float midiNoteFreq;
Parameter lfoShapeParam, lfoRateParam, waveformParam, oscillatorLfoModParam, cutoffParam, resonanceParam, svfAdsrParam, svfLfoModParam,
    pulseWidthParam, pulseWidthLfoModParam, subOscLevelParam, attackParam, decayParam, sustainParam, releaseParam;

void UpdateLfo()
{
    float lfoRate = lfoRateParam.Process();
    lfo.SetFreq(lfoRate);
}

void UpdateWaveform()
{
    // I expect there's a better way of doing this.
    float waveformValue = waveformParam.Process();

    if (waveformValue >= 0 && waveformValue < 1.0f)
    {
        oscillator.SetWaveform(Oscillator::WAVE_TRI);
    }
    else if (waveformValue >= 1.0f && waveformValue < 2.0f)
    {
        oscillator.SetWaveform(Oscillator::WAVE_SAW);
    }
    else
    {
        oscillator.SetWaveform(Oscillator::WAVE_SQUARE);
    }

    float subOscLevel = subOscLevelParam.Process();
}

void UpdateAdsr()
{
    float attackTime = attackParam.Process();
    float decayTime = decayParam.Process();
    float sustainLevel = sustainParam.Process();
    float releaseTime = releaseParam.Process();
    adsr.SetTime(ADSR_SEG_ATTACK, attackTime);
    adsr.SetTime(ADSR_SEG_DECAY, decayTime);
    adsr.SetSustainLevel(sustainLevel);
    adsr.SetTime(ADSR_SEG_RELEASE, releaseTime);
}

void UpdateSvf()
{
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    UpdateLfo();
    UpdateWaveform();
    UpdateAdsr();
    // UpdateSvf();

    float cutOffFrequency = cutoffParam.Process();
    float resonance = resonanceParam.Process();
    float svfAdsr = svfAdsrParam.Process();
    float svfLfoMod = svfLfoModParam.Process();
    float oscillatorLfoMod = oscillatorLfoModParam.Process();
    float pulseWidth = pulseWidthParam.Process();
    float pulseWidthLfoMod = pulseWidthLfoModParam.Process();
    float subOscLevel = subOscLevelParam.Process();
    svf.SetRes(resonance);

    float adsrOut, lfoOut, oscillatorOut, subOscOut, filterOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        lfo.SetAmp(oscillatorLfoMod);
        lfoOut = lfo.Process();
        oscillator.SetAmp(adsrOut / 10);
        oscillator.SetFreq(midiNoteFreq + lfoOut);

        subOsc.SetAmp(adsrOut / 20 * subOscLevel);
        subOsc.SetFreq((midiNoteFreq / 2) + lfoOut);

        lfo.SetAmp(pulseWidthLfoMod);
        lfoOut = lfo.Process();
        oscillator.SetPw(pulseWidth + lfoOut);

        oscillatorOut = oscillator.Process();
        subOscOut = subOsc.Process();

        lfo.SetAmp(svfLfoMod);
        lfoOut = lfo.Process();
        svf.SetFreq(cutOffFrequency + (maxCutoffFrequency / 2 * adsrOut * svfAdsr) + (maxCutoffFrequency / 2 * lfoOut));

        svf.Process(oscillatorOut + subOscOut);
        filterOut = svf.Low();
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
    oscillator.Init(sampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_SAW);
    oscillator.SetAmp(0.1f);
    oscillator.SetPw(0.5f);
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
    midiChannel = 0;
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitParameters(float sampleRate)
{
    lfoRateParam.Init(potentiometerArray.analogControl[0], 0.25f, 20.0f, Parameter::LOGARITHMIC);
    waveformParam.Init(potentiometerArray.analogControl[1], 0, 3.0f, Parameter::LINEAR);
    oscillatorLfoModParam.Init(potentiometerArray.analogControl[2], 0, 100.0f, Parameter::LINEAR);
    pulseWidthParam.Init(potentiometerArray.analogControl[3], 0.005f, 0.51f, Parameter::LINEAR);
    pulseWidthLfoModParam.Init(potentiometerArray.analogControl[4], 0, 0.5f, Parameter::LINEAR);
    subOscLevelParam.Init(potentiometerArray.analogControl[5], 0, 1.0f, Parameter::LINEAR);
    cutoffParam.Init(potentiometerArray.analogControl[8], 0, sampleRate / 3, Parameter::LINEAR);
    resonanceParam.Init(potentiometerArray.analogControl[9], 0, 1.0f, Parameter::LINEAR);
    svfAdsrParam.Init(potentiometerArray.analogControl[10], 0, 1.0f, Parameter::LINEAR);
    svfLfoModParam.Init(potentiometerArray.analogControl[11], 0, 1.0f, Parameter::LINEAR);
    attackParam.Init(potentiometerArray.analogControl[12], 0.001f, 1.0f, Parameter::LINEAR);
    decayParam.Init(potentiometerArray.analogControl[13], 0.025f, 1.0f, Parameter::LINEAR);
    sustainParam.Init(potentiometerArray.analogControl[14], 0., 1.0f, Parameter::LINEAR);
    releaseParam.Init(potentiometerArray.analogControl[15], 0, 1.0f, Parameter::LINEAR);
}

void HandleMidiMessage(MidiEvent m)
{
    if (m.channel == midiChannel)
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
