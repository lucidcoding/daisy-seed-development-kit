#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/NavigationKeypad.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::hardware::navigationKeypad::drivers;

#define LED_COUNT 35
#define PROCESS_INTERVAL_US 250
#define LED_CHANGE_STEPS 4000

static DaisySeed hardware;
NavigationKeypad navigationKeypad;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    navigationKeypad.Init();
    hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");
    uint32_t lastTicks;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicks > (NAVIGATION_KEYPAD_SCAN_INTERVAL_US * ticksPerUs))
        {
            lastTicks = currentTicks;
            uint8_t lastKeyPress = navigationKeypad.ScanKeys(currentTicks);

            if (lastKeyPress != NAVIGATION_KEYPAD_NO_KEY_PRESS)
            {
                hardware.PrintLine("Key press: %d", lastKeyPress);
            }
        }
    }
}