#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hardware;

AnalogControl knob;
Parameter param;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
}

int main(void) 
{
	hardware.Configure();
    hardware.Init();
	hardware.StartLog(false);

	dsy_gpio s0;
    s0.pin = hardware.GetPin(19);
	s0.mode = DSY_GPIO_MODE_OUTPUT_PP;
	s0.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&s0);
	dsy_gpio_write(&s0, true); 

	dsy_gpio s1;
    s1.pin = hardware.GetPin(20);
	s1.mode = DSY_GPIO_MODE_OUTPUT_PP;
	s1.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&s1);
	dsy_gpio_write(&s1, true); 

	dsy_gpio s2;
    s2.pin = hardware.GetPin(21);
	s2.mode = DSY_GPIO_MODE_OUTPUT_PP;
	s2.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&s2);
	dsy_gpio_write(&s2, true); 

	AdcChannelConfig adcConfig;
	adcConfig.InitSingle(hardware.GetPin(18));
	hardware.adc.Init(&adcConfig, 1);
	hardware.adc.Start();

	while(1)
	{
		int val = hardware.adc.Get(0);
		hardware.PrintLine("Value: %d", val);
        System::Delay(1000);
	}

}
