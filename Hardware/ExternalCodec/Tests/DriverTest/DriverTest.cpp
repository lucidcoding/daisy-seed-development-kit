#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/ExternalCodec.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
static Oscillator osc;
ExternalCodec externalCodec;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float sig;
    for (size_t i = 0; i < size; i += 2)
    {
        sig = osc.Process();

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    hardware.Configure();
    hardware.Init();
    externalCodec.Init(&hardware);
    hardware.SetAudioBlockSize(4);
    sample_rate = hardware.AudioSampleRate();
    osc.Init(sample_rate);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    // start callback
    hardware.StartAudio(AudioCallback);

    while (1)
    {
    }
}