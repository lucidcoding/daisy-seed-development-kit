#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hardware;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    hardware.StartAudio(AudioCallback);
    dsy_gpio gate_output;
    gate_output.pin = hardware.GetPin(24);
    gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
    gate_output.pull =  DSY_GPIO_PULLUP; //DSY_GPIO_NOPULL;
    dsy_gpio_init(&gate_output);

    while (1)
    {
        dsy_gpio_write(&gate_output, true);  // set high
        System::Delay(15);
        dsy_gpio_write(&gate_output, false);  // set high
        System::Delay(200);
    }
}
