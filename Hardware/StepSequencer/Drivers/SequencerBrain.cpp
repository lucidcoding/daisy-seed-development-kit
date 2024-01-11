#include "stdint.h"
#include "SequencerBrain.h"
#include "NoteEvent.h"
#include "Debug.h"

namespace developmentKit::stepSequencer
{
    void SequencerBrain::Init()
    {
        stepCount = 16;
        currentStep = 0;
        stepInterval = 500; // 500 is approx 120 bpm.
        gateLength = stepInterval / 2;
        tick = stepInterval;
        mode = STEP_SEQUENCER_MODE_STOP;
        gateOn = false;

        steps[0].note = 0;
        steps[0].gate = true;

        steps[1].gate = false;

        steps[2].note = 0;
        steps[2].gate = true;
        steps[2].slide = true;

        steps[3].note = 0;
        steps[3].gate = true;

        steps[4].note = 0;
        steps[4].gate = true;

        steps[5].note = 0;
        steps[5].gate = true;

        steps[6].gate = false;

        steps[7].note = 0;
        steps[7].gate = true;

        steps[8].note = 0;
        steps[8].gate = true;

        steps[9].gate = false;

        steps[10].note = 0;
        steps[10].gate = true;

        steps[11].note = 0;
        steps[11].gate = true;

        /*steps[12].gate = false;

        steps[13].note = 0;
        steps[13].gate = true;

        steps[14].note = 0;
        steps[14].slide = true;
        steps[14].gate = true;

        steps[15].note = 0;
        steps[15].gate = true;*/

        steps[12].gate = false;
        steps[13].gate = false;
        steps[14].gate = false;
        steps[15].gate = false;
    }

    void SequencerBrain::UpdateLedsForCurrentStep()
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

    void SequencerBrain::StartStepRecording()
    {
        mode = STEP_SEQUENCER_MODE_STEP_REC;
        currentStep = 0;
        UpdateLedsForCurrentStep();
    }

    void SequencerBrain::SetKeys(uint8_t keys)
    {
        lastKeyPress = keys;
    }

    uint64_t SequencerBrain::GetLedStates()
    {
        uint64_t returnValue = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_NUMBER_OF_LEDS; ledIndex++)
        {
            returnValue = returnValue | ((leds[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void SequencerBrain::ActivateCurrentStep()
    {
        DEBUG("Activating step: " << (uint16_t)currentStep << ", gate: " << steps[currentStep].gate);
        tick = stepInterval;
        UpdateLedsForCurrentStep();

        if (steps[currentStep].gate)
        {
            DEBUG("Open gate");
            gateCount = 0;
            gateOn = true;
        }
        else
        {
             DEBUG("No gate");
        }
    }

    void SequencerBrain::Process(uint32_t currentProcessTimeUs)
    {
        if (lastKeyPress != STEP_SEQUENCER_NO_KEY_PRESS)
        {
            leds[lastKeyPress] = true;

            if (lastKeyPress == STEP_SEQUENCER_KEYS_PLAY)
            {
                DEBUG("Play pressed");

                if (mode == STEP_SEQUENCER_MODE_PLAY)
                {
                    DEBUG("Stopping");

                    mode = STEP_SEQUENCER_MODE_STOP;
                    // gateOn = false;
                }
                else if (mode == STEP_SEQUENCER_MODE_STOP)
                {
                    DEBUG("Playing");
                    currentStep = 0;
                    ActivateCurrentStep();

                    mode = STEP_SEQUENCER_MODE_PLAY;
                }
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

            lastKeyPress = STEP_SEQUENCER_NO_KEY_PRESS;
        }

        DEBUG("Tick: " << tick << ", Mode: " << (uint16_t)mode);

        if (mode == STEP_SEQUENCER_MODE_PLAY && --tick <= 0)
        {
            DEBUG("Processing tick: " << tick);
            currentStep = (currentStep + 1) % stepCount;
            ActivateCurrentStep();

        }

        if (gateOn)
        {
            DEBUG("Gate on, when to stop:" << (stepInterval - gateLength));

            if (tick < (stepInterval - gateLength))
            {
                DEBUG("Close gate");

                if (!steps[currentStep].slide)
                {
                    gateOn = false;
                }

                if (steps[currentStep].slide && mode != STEP_SEQUENCER_MODE_PLAY)
                {
                    gateOn = false;
                }
            }
        }
    }

    uint8_t SequencerBrain::GetNoteFromKeyPressed(uint8_t keyPressed)
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

    bool SequencerBrain::GetGate()
    {
        return gateOn;
    }

    void SequencerBrain::SetStepInterval(uint8_t newStepInterval)
    {
        stepInterval = newStepInterval;
        gateLength = stepInterval / 2;

        /*if (tick > stepInterval)
        {
            tick = stepInterval;
        }*/
        tick = 0;
    }

    void SequencerBrain::SetSteps(Step newSteps[16])
    {
        for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
        {
            steps[stepIndex].gate = newSteps[stepIndex].gate;
            steps[stepIndex].note = newSteps[stepIndex].note;
            steps[stepIndex].octaveDown = newSteps[stepIndex].octaveDown;
            steps[stepIndex].octaveUp = newSteps[stepIndex].octaveUp;
            steps[stepIndex].accent = newSteps[stepIndex].accent;
            steps[stepIndex].slide = newSteps[stepIndex].slide;
        }
    }

    uint8_t SequencerBrain::GetCurrentStepIndex()
    {
        return currentStep;
    }

    uint8_t SequencerBrain::GetMode()
    {
        return mode;
    }

    Step SequencerBrain::GetCurrentStep()
    {
        return steps[currentStep];
    }
}