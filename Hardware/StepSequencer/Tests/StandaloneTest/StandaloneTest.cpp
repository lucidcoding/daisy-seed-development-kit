#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"
#include "../../Drivers/Keys.h"
#include "../../Drivers/Leds.h"
#include "../../Drivers/StepSequencer.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;

static DaisySeed hardware;
Mcp23017 mcp;
Mcp23017 mcp2;
bool ledOn = true;
bool stableState[24];
bool lastState[24];
uint32_t lastDebounceTime[24];
uint32_t debounceDelay = 1000;
Keys keys;
Leds leds;
StepSequencer stepSequencer(&keys, &leds);

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
    hardware.PrintLine("Starting...");

    stepSequencer.seed = &hardware;
    stepSequencer.Init();

 

    while (1)
    {
        stepSequencer.Process(System::GetUs());
    }
}
/*int main(void)
{
    const uint8_t ledMax = 22;
    uint8_t currentLed = 0;
    const uint16_t ledChangeCountdownMax = 2000;
    uint16_t ledChangeCountdown = ledChangeCountdownMax;
    uint32_t lastProcessTimeUs;
    uint32_t processIntervalUs = 500;

    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");

    keys.Init();
    leds.Init();

    while (1)
    {
        uint32_t currentProcessTimeUs = System::GetUs();

        if (currentProcessTimeUs - lastProcessTimeUs > processIntervalUs)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            if (ledChangeCountdown-- < 1)
            {
                ledChangeCountdown = ledChangeCountdownMax;
                currentLed = (currentLed + 1) % ledMax;

                for (uint8_t ledToSet = 0; ledToSet <= ledMax; ledToSet++)
                {
                    leds.SetLed(ledToSet, ledToSet == currentLed);
                }
            }

            uint8_t retVal = keys.ScanNextColumn(currentProcessTimeUs);

            if (retVal < 255)
            {
                hardware.PrintLine("RetVal: %d", retVal);
            }

            leds.ScanNextColumn();
        }
    }
}*/