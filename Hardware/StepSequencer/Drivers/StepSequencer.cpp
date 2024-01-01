#include "stdint.h"
#include "StepSequencer.h"

namespace developmentKit::stepSequencer
{
    void StepSequencer::Init()
    {
        stepCount = 16;
        currentStep = 0;
        stepInterval = 250;
        tick = stepInterval - 1;
        mode = STEP_SEQUENCER_MODE_STOP;

        steps[0].accent = true;
        steps[1].note = 1;
        steps[3].note = 4;
        steps[3].accent = true;
        steps[5].octaveUp = true;
        steps[6].note = 1;
        steps[6].slide = true;
        steps[8].note = 10;
        steps[10].note = 9;
        steps[10].accent = true;
        steps[12].note = 3;
        steps[12].octaveDown = true;
        steps[13].note = 3;
        steps[14].note = 1;
        steps[15].octaveUp = true;
    }

    void StepSequencer::UpdateLedsForCurrentStep()
    {
        Step step = steps[currentStep];

        for (uint8_t ledToSet = 0; ledToSet <= 15; ledToSet++)
        {
            leds[ledToSet] = (ledToSet == noteToLedLookup[step.note]);
        }

        leds[17] = step.octaveDown;
        leds[18] = step.octaveUp;
        leds[19] = step.accent;
        leds[20] = step.slide;
    }

    void StepSequencer::StartStepRecording()
    {
        mode = STEP_SEQUENCER_MODE_STEP_REC;
        currentStep = 0;
        UpdateLedsForCurrentStep();
    }

    void StepSequencer::SetKeys(uint8_t keys)
    {
        lastKeyPress = keys;
    }

    uint64_t StepSequencer::GetLedStates()
    {
        uint64_t returnValue = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < 64; ledIndex++)
        {
            returnValue = returnValue | ((leds[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void StepSequencer::Process(uint32_t currentProcessTimeUs)
    {

        if (mode == STEP_SEQUENCER_MODE_PLAY && tick++ >= stepInterval)
        {
            tick = 0;

            UpdateLedsForCurrentStep();
            currentStep = (currentStep + 1) % stepCount;
        }

        if (lastKeyPress < 255)
        {
            leds[lastKeyPress] = true;

            if (lastKeyPress == 6)
            {
                mode = (mode == STEP_SEQUENCER_MODE_PLAY) ? STEP_SEQUENCER_MODE_STOP : STEP_SEQUENCER_MODE_PLAY;
            }

            if (lastKeyPress == 7)
            {
                StartStepRecording();
            }

            if (mode == STEP_SEQUENCER_MODE_STEP_REC)
            {
                if (lastKeyPress == 21)
                {
                    if (currentStep > 0)
                    {
                        currentStep--;
                        UpdateLedsForCurrentStep();
                    }
                }

                if (lastKeyPress == 22)
                {
                    if (currentStep < (stepCount - 1))
                    {
                        currentStep++;
                        UpdateLedsForCurrentStep();
                    }
                }
            }

            if (mode == STEP_SEQUENCER_MODE_STEP_REC)
            {
                uint8_t note = getNoteFromKeyPressed(lastKeyPress);

                if(note != 255)
                {
                    steps[currentStep].note = note;
                }

                if (lastKeyPress == 17)
                {
                    steps[currentStep].octaveDown = !steps[currentStep].octaveDown;
                }

                if (lastKeyPress == 18)
                {
                    steps[currentStep].octaveUp = !steps[currentStep].octaveUp;
                }

                if (lastKeyPress == 19)
                {
                    steps[currentStep].accent = !steps[currentStep].accent;
                }

                if (lastKeyPress == 20)
                {
                    steps[currentStep].slide = !steps[currentStep].slide;
                }

                UpdateLedsForCurrentStep();
            }
        }
    }

    uint8_t StepSequencer::getNoteFromKeyPressed(uint8_t keyPressed)
    {
        for (uint8_t currentIndex = 0; currentIndex < 13; currentIndex++)
        {
            if(noteToLedLookup[currentIndex] == keyPressed)
            {
                return currentIndex;
            }
        }

        return 255;
    }
}