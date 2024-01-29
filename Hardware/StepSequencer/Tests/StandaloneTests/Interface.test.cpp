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
u_int16_t ledChangeCountdown;

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
    ledChangeCountdown = LED_CHANGE_STEPS;

    while (1)
    {
        uint32_t currentProcessTimeUs = System::GetUs();
        ledChangeCountdown = (ledChangeCountdown - 1) % LED_CHANGE_STEPS;

        if (ledChangeCountdown == 0)
        {
            ledIndex = (ledIndex + 1) % LED_COUNT;
        }

        interface.ScanNextLedsColumn(0x00 | (1 << ledIndex), currentProcessTimeUs);
        uint32_t lastKeyPress = interface.ScanNextKeysColumn(currentProcessTimeUs);

        if (lastKeyPress != STEP_SEQUENCER_KEYS_NO_KEY_PRESS)
        {
            hardware.PrintLine("Key press: %d", lastKeyPress);
        }
    }
}
