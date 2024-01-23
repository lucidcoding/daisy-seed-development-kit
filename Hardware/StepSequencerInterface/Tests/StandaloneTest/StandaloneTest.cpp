#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Drivers/StepSequencer.h"
#include "../../Drivers/StepSequencerInterface.h";

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::stepSequencer;

#define LED_COUNT 23
#define PROCESS_INTERVAL_US 250
#define LED_CHANGE_STEPS 4000

static DaisySeed hardware;
//Keys keys;
//Leds leds;
StepSequencerInterface stepSequencerInterface;
u_int8_t ledIndex;
u_int16_t ledChangeCountdown;
uint32_t lastProcessTimeUs;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    //leds.Init();
    //keys.Init();
    stepSequencerInterface.Init();
    float sampleRate = hardware.AudioSampleRate();
    //hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");
    ledIndex = 0;
    ledChangeCountdown = LED_CHANGE_STEPS;

    while (1)
    {
        uint32_t currentProcessTimeUs = System::GetUs();

        if (currentProcessTimeUs - lastProcessTimeUs > PROCESS_INTERVAL_US)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            ledChangeCountdown = (ledChangeCountdown - 1) % LED_CHANGE_STEPS;

            if (ledChangeCountdown == 0)
            {
                ledIndex = (ledIndex + 1) % LED_COUNT;
            }

            //leds.SetLeds(0x00 | (1 << ledIndex));
            //leds.ScanNextColumn();
            stepSequencerInterface.ScanNextLedsColumn(0x00 | (1 << ledIndex));
            //uint32_t lastKeyPress = keys.ScanNextColumn(currentProcessTimeUs);
            uint32_t lastKeyPress = stepSequencerInterface.ScanNextKeysColumn(currentProcessTimeUs);

            if (lastKeyPress != STEP_SEQUENCER_NO_KEY_PRESS)
            {
                hardware.PrintLine("Key press: %d", lastKeyPress);
            }
        }
    }
}
