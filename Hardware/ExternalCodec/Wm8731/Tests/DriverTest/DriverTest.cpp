#include "daisy_seed.h"
#include "daisysp.h"
#include "../../Drivers/ExternalCodec.h"

using namespace daisy;
using namespace daisysp;
using namespace developmentKit::hardware::externalCodec::drivers;

DaisySeed hardware;
ExternalCodec externalCodec;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = in[2][i];
        out[1][i] = in[3][i];
        out[2][i] = in[0][i];
        out[3][i] = in[1][i];
    }
}

int main(void)
{
    hardware.Init();
    externalCodec.Init(&hardware);
    hardware.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hardware.StartAudio(AudioCallback);
    while (1)
    {
    }
}
