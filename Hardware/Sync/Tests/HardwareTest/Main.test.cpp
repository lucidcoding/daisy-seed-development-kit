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
    gate_output.pull = DSY_GPIO_PULLUP;
    dsy_gpio gate_input;
    gate_input.pin = hardware.GetPin(25);
    gate_input.mode = DSY_GPIO_MODE_INPUT;
    gate_input.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&gate_output);
    dsy_gpio_init(&gate_input);
    uint32_t ticksPerMs = System::GetTickFreq() / 1000;
    const uint32_t waitTimeMs = 235;
    const uint32_t pulseTimeMs = 150;
    uint32_t lastPulseTicks = System::GetTick();
    uint32_t nextEventMs = waitTimeMs;
    bool pulseOn;
    const uint32_t ledOnTimeMs = 100;
    uint32_t ledOnTicks = System::GetTick();
    bool currentPulseInRead = false;
    bool lastPulseInRead = false;
    bool ledOn;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastPulseTicks > (nextEventMs * ticksPerMs))
        {
            lastPulseTicks = currentTicks;

            if (pulseOn)
            {
                pulseOn = false;
                nextEventMs = waitTimeMs;
                dsy_gpio_write(&gate_output, true);
            }
            else
            {
                pulseOn = true;
                nextEventMs = pulseTimeMs;
                dsy_gpio_write(&gate_output, false);
            }
        }

        currentPulseInRead = !dsy_gpio_read(&gate_input);

        if (currentPulseInRead && !lastPulseInRead)
        {
            ledOn = true;
            hardware.SetLed(ledOn);
            hardware.PrintLine("LedOn");
            ledOnTicks = currentTicks;
        }

        if (ledOn && (currentTicks - ledOnTicks > (ledOnTimeMs * ticksPerMs)))
        {
            ledOnTicks = currentTicks;
            ledOn = false;
            hardware.SetLed(ledOn);
            hardware.PrintLine("LedOff");
        }

        lastPulseInRead = currentPulseInRead;

        // bool currentRead = dsy_gpio_read(&gate_input);
        //  hardware.PrintLine("currentRead: %d", currentRead);

        /*if (lastRead == true && currentRead == false)
        {
            ledState = !ledState;
        }

        lastRead = currentRead;

        hardware.SetLed(ledState);*/
    }
}
