#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::hardware::potentiometerArray::drivers;

static DaisySeed hardware;
PotentiometerArray potentiometerArray;
Parameter parameters[16];
float parameterValues[16];

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();

    for (uint8_t i = 0; i < 16; i++)
    {
        parameterValues[i] = parameters[i].Process();
    }
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitParameters(float sampleRate)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        parameters[i].Init(potentiometerArray.analogControl[i], 0, 1.0f, Parameter::LINEAR);
    }
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    InitPotentiometerArray();
    InitParameters(sampleRate);
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);

    while (1)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            hardware.Print("%d:%3.4f ", i, parameterValues[i]);
        }

        hardware.PrintLine("");
        System::Delay(1000);
    }
}
