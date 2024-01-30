#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/Interface.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;

#define LED_COUNT 23
#define PROCESS_INTERVAL_US 250
#define LED_CHANGE_STEPS 4000

static DaisySeed hardware;
Interface interface;
u_int8_t ledIndex;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    interface.Init();
    hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");
    ledIndex = 0;
    uint32_t ticksPerUs = System::GetTickFreq() / 1000000;
    uint32_t lastTicks = System::GetTick();

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicks >  (1000000 * ticksPerUs))
        {
            lastTicks = currentTicks;
            ledIndex = (ledIndex + 1) % LED_COUNT;
        }

        interface.ScanLeds(0 | (1 << ledIndex), currentTicks);
        uint32_t lastKeyPress =  interface.ScanKeys(currentTicks);

        if (lastKeyPress != STEP_SEQUENCER_KEYS_NO_KEY_PRESS)
        {
            hardware.PrintLine("Key press: %d", lastKeyPress);
        }
    }
}
