#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/Sync.h"

using namespace daisy;
using namespace daisysp;
using namespace developmentKit::hardware::sync::drivers;

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
    Sync sync;
    sync.Init(&hardware);
    uint32_t ticksPerMs = System::GetTickFreq() / 1000;
    const uint32_t ledOnTimeMs = 100;
    uint32_t ledOnTicks = System::GetTick();
    bool currentPulseInRead = false;
    bool ledOn;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();
        currentPulseInRead = sync.Poll();

        if (currentPulseInRead)
        {
            ledOn = true;
            hardware.SetLed(ledOn);
            ledOnTicks = currentTicks;
        }

        if (ledOn && (currentTicks - ledOnTicks > (ledOnTimeMs * ticksPerMs)))
        {
            ledOnTicks = currentTicks;
            ledOn = false;
            hardware.SetLed(ledOn);
        }
    }
}
