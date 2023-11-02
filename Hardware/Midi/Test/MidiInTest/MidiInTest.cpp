#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hardware;
MidiUartHandler midi;
Oscillator oscillator;
Adsr adsr;
bool gate;
int midiChannel;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        oscillator.SetAmp(adsrOut / 10);
        oscillatorOut = oscillator.Process();
        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    oscillator.Init(sampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
    oscillator.SetAmp(0.1);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.1);
}

void InitMidi()
{
    MidiUartHandler::Config midiConfig;
    midi.Init(midiConfig);
    midiChannel = 0;
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
                oscillator.SetFreq(mtof(p.note));
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
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitMidi();
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
