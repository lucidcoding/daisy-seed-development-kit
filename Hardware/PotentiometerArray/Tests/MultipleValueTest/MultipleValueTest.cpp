#include "daisysp.h"
#include "daisy_seed.h"
#include "../../PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
PotentiometerArray potentiometerArray;
float maxCutoffFrequency;
Parameter cutoffParam, resonanceParam, svfAdsrParam, attackParam, decayParam, sustainParam, releaseParam;
float attackTime, decayTime, sustainLevel, releaseTime;

void UpdateAdsr()
{
    attackTime = attackParam.Process();
    decayTime = decayParam.Process();
    sustainLevel = sustainParam.Process();
    releaseTime = releaseParam.Process();
}

void UpdateSvf()
{
    float cutOffFrequency = cutoffParam.Process();
    float resonance = resonanceParam.Process();
    float svfAdsr = svfAdsrParam.Process();
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    UpdateAdsr();
    UpdateSvf();
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitParameters(float sampleRate)
{
    cutoffParam.Init(potentiometerArray.analogControl[1], 0, sampleRate / 3, Parameter::LINEAR);
    resonanceParam.Init(potentiometerArray.analogControl[2], 0, 1.0f, Parameter::LINEAR);
    svfAdsrParam.Init(potentiometerArray.analogControl[3], 0, 1.0f, Parameter::LINEAR);
    attackParam.Init(potentiometerArray.analogControl[8], 0, 1.0f, Parameter::LINEAR);
    decayParam.Init(potentiometerArray.analogControl[9], 0, 1.0f, Parameter::LINEAR);
    sustainParam.Init(potentiometerArray.analogControl[10], 0, 1.0f, Parameter::LINEAR);
    releaseParam.Init(potentiometerArray.analogControl[11], 0, 1.0f, Parameter::LINEAR);
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

    int countdown = 60000;
    while (1)
    {

        for (int i = 0; i < 16; i++)
        {
            // seed.Print("%d:%d\t", i, (potentiometerArray.analogControl[i].GetRawValue()));
            /*seed.Print("c:%d ", cutoffParam.Value() * 10000);
            seed.Print("r:%d ", resonanceParam.Value() * 10000);
            seed.Print("e:%d ", svfAdsrParam.Value() * 10000);
            seed.Print("a:%d ", attackParam.Value() * 10000);
            seed.Print("d:%d ", decayParam.Value() * 10000);
            seed.Print("s:%d ", sustainParam.Value() * 10000);
            seed.Print("r:%d ", releaseParam.Value() * 10000);*/
        }

        /*char str[30];
        sprintf(str, "a: %3.5f ", attackTime);
        seed.Print(str);
        sprintf(str, "d:%3.5f ", decayTime);
        seed.Print(str);
        sprintf(str, "s:%3.5f ", sustainLevel);
        seed.Print(str);
        sprintf(str, "r:%3.5f ", releaseTime);
        seed.Print(str);*/

        hardware.Print("a:%3.5f ", attackTime);
        hardware.Print("d:%3.5f ", decayTime);
        hardware.Print("s:%3.5f ", sustainLevel);
        hardware.Print("r:%3.5f ", releaseTime);

        hardware.PrintLine("");
        System::Delay(1000);
    }
}
