#include "stdint.h"
#include "StepSequencer.h"

namespace developmentKit::stepSequencer
{
    void StepSequencer::Init()
    {
        stepCount = 16;
        currentStep = 0;
        stepInterval = 500;
        tick = stepInterval - 1;
        mode = STEP_SEQUENCER_MODE_STOP;
        hasStepEvent = false;

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

        for (uint8_t ledToSet = 0; ledToSet < 17; ledToSet++)
        {
            leds[ledToSet] = (ledToSet == noteToLedLookup[step.note]);
        }

        leds[STEP_SEQUENCER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        leds[STEP_SEQUENCER_LEDS_OCTAVE_UP] = step.octaveUp;
        leds[STEP_SEQUENCER_LEDS_ACCENT] = step.accent;
        leds[STEP_SEQUENCER_LEDS_SLIDE] = step.slide;
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

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_NUMBER_OF_LEDS; ledIndex++)
        {
            returnValue = returnValue | ((leds[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void StepSequencer::Process(uint32_t currentProcessTimeUs)
    {
        hasStepEvent = false;

        if (mode == STEP_SEQUENCER_MODE_PLAY && tick++ >= stepInterval)
        {
            tick = 0;
            UpdateLedsForCurrentStep();
            currentStep = (currentStep + 1) % stepCount;
            hasStepEvent = true;
        }

        if (lastKeyPress != STEP_SEQUENCER_NO_KEY_PRESS)
        {
            leds[lastKeyPress] = true;

            if (lastKeyPress == STEP_SEQUENCER_KEYS_PLAY)
            {
                mode = (mode == STEP_SEQUENCER_MODE_PLAY) ? STEP_SEQUENCER_MODE_STOP : STEP_SEQUENCER_MODE_PLAY;
            }

            if (lastKeyPress == STEP_SEQUENCER_KEYS_REC)
            {
                StartStepRecording();
            }

            if (mode == STEP_SEQUENCER_MODE_STEP_REC)
            {
                if (lastKeyPress == STEP_SEQUENCER_KEYS_BACK)
                {
                    if (currentStep > 0)
                    {
                        currentStep--;
                        UpdateLedsForCurrentStep();
                    }
                }

                if (lastKeyPress == STEP_SEQUENCER_KEYS_NEXT)
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
                uint8_t note = GetNoteFromKeyPressed(lastKeyPress);

                if (note != STEP_SEQUENCER_NOT_NOTE_KEY)
                {
                    steps[currentStep].note = note;
                }

                if (lastKeyPress == STEP_SEQUENCER_KEYS_OCTAVE_DOWN)
                {
                    steps[currentStep].octaveDown = !steps[currentStep].octaveDown;
                }

                if (lastKeyPress == STEP_SEQUENCER_KEYS_OCTAVE_UP)
                {
                    steps[currentStep].octaveUp = !steps[currentStep].octaveUp;
                }

                if (lastKeyPress == STEP_SEQUENCER_KEYS_ACCENT)
                {
                    steps[currentStep].accent = !steps[currentStep].accent;
                }

                if (lastKeyPress == STEP_SEQUENCER_KEYS_SLIDE)
                {
                    steps[currentStep].slide = !steps[currentStep].slide;
                }

                UpdateLedsForCurrentStep();
            }
        }
    }

    uint8_t StepSequencer::GetNoteFromKeyPressed(uint8_t keyPressed)
    {
        for (uint8_t currentIndex = 0; currentIndex < STEP_SEQUENCER_NUMBER_OF_NOTE_KEYS; currentIndex++)
        {
            if (noteToLedLookup[currentIndex] == keyPressed)
            {
                return currentIndex;
            }
        }

        return STEP_SEQUENCER_NOT_NOTE_KEY;
    }

    bool StepSequencer::HasStepEvent()
    {
        return hasStepEvent;
    }

    Step StepSequencer::GetCurrentStep()
    {
        return steps[currentStep];
    }
}