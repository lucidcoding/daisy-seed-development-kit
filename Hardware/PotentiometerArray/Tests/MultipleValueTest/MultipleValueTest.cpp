#include "daisysp.h"
#include "daisy_seed.h"
#include "../../PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
PotentiometerArray potentiometerArray;
Parameter param0, param1, param2, param3;
float param0Value, param1Value, param2Value, param3Value;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    param0Value = param0.Process();
    param1Value = param1.Process();
    param2Value = param2.Process();
    param3Value = param3.Process();
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitParameters(float sampleRate)
{
    param0.Init(potentiometerArray.analogControl[0], 0, 1.0f, Parameter::LINEAR);
    param1.Init(potentiometerArray.analogControl[1], 0, 1.0f, Parameter::LINEAR);
    param2.Init(potentiometerArray.analogControl[2], 0, 1.0f, Parameter::LINEAR);
    param3.Init(potentiometerArray.analogControl[3], 0, 1.0f, Parameter::LINEAR);
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
        hardware.Print("p1:%3.5f ", param0Value);
        hardware.Print("p2:%3.5f ", param1Value);
        hardware.Print("p3:%3.5f ", param2Value);
        hardware.Print("p4:%3.5f ", param3Value);
        hardware.PrintLine("");
        System::Delay(1000);
    }
}
