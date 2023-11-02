#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Hardware/PotentiometerArray/PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
static Metro tick;
bool gate;
Oscillator osc;
Adsr adsr;
PotentiometerArray potentiometerArray;

void UpdateParameters()
{
    potentiometerArray.Process();
    float release = potentiometerArray.parameter[5].Process();
    adsr.SetTime(ADSR_SEG_RELEASE, release);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    UpdateParameters();
    float osc_out, env_out;

    for (size_t i = 0; i < size; i += 2)
    {
        if (tick.Process())
        {
            gate = !gate;
        }

        env_out = adsr.Process(gate);
        osc.SetAmp(env_out / 10);
        osc_out = osc.Process();

        out[i] = osc_out;
        out[i + 1] = osc_out;
    }
}

int main(void)
{
    float sample_rate;
    hardware.Configure();
    hardware.Init();
    sample_rate = hardware.AudioSampleRate();
    adsr.Init(sample_rate);
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
    osc.SetAmp(0.1);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.1);
    tick.Init(1.0f, sample_rate);
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);
    while (1)
    {
    }
}
