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
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    hardware.StartAudio(AudioCallback);
    dsy_gpio gate_output;
    gate_output.pin = hardware.GetPin(24);
    gate_output.mode = DSY_GPIO_MODE_OUTPUT_PP;
    gate_output.pull =  DSY_GPIO_PULLUP; //DSY_GPIO_NOPULL;

    
    dsy_gpio gate_input;
    gate_input.pin = hardware.GetPin(25);
    gate_input.mode = DSY_GPIO_MODE_INPUT;
    gate_input.pull =DSY_GPIO_PULLUP;//  DSY_GPIO_PULLUP; //DSY_GPIO_NOPULL;

    dsy_gpio_init(&gate_output);
    dsy_gpio_init(&gate_input);

    bool lastRead = true;
    bool ledState = false;

    while (1)
    {
        /*dsy_gpio_write(&gate_output, true);  // set high
        System::Delay(15);
        dsy_gpio_write(&gate_output, false);  // set high
        System::Delay(200);*/
        //dsy_gpio_read(&gate_input);
        bool currentRead = dsy_gpio_read(&gate_input);
        //hardware.PrintLine("currentRead: %d", currentRead);

        if(lastRead == true && currentRead == false)
        {
            ledState = !ledState;
        }

        lastRead = currentRead;

        hardware.SetLed(ledState);
        
    }
}
