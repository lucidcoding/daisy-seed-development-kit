#include "stdint.h"
#include "SequencerBrain.h"
#include "NoteEvent.h"
#include "DEBUG.h"

namespace developmentKit::stepSequencer
{
    void SequencerBrain::Init()
    {
        stepCount = 16;
        currentStepIndex = 0;
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

        steps[7].note = 7;
        steps[7].gate = true;

        steps[8].note = 7;
        steps[8].gate = true;

        steps[9].gate = false;

        steps[10].note = 7;
        steps[10].gate = true;

        steps[11].note = 12;
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
        UpdateLeds();
    }

    void SequencerBrain::UpdateLeds()
    {
        Step step = steps[currentStepIndex];

        for (uint8_t ledToSet =STEP_SEQUENCER_LEDS_C_SHARP; ledToSet <= STEP_SEQUENCER_LEDS_A_SHARP; ledToSet++)
        {
            if (steps[currentStepIndex].gate)
            {
                leds[ledToSet] = (ledToSet == noteToLedLookup[step.note]);
            }
        }

        leds[STEP_SEQUENCER_LEDS_FUNC] = false;

        if (mode == STEP_SEQUENCER_MODE_PLAY)
        {
            leds[STEP_SEQUENCER_LEDS_PLAY] = true;
            leds[STEP_SEQUENCER_LEDS_REC] = false;
        }
        else if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            leds[STEP_SEQUENCER_LEDS_PLAY] = false;
            leds[STEP_SEQUENCER_LEDS_REC] = true;
        }
        else if (mode == STEP_SEQUENCER_MODE_STOP)
        {
            leds[STEP_SEQUENCER_LEDS_PLAY] = false;
            leds[STEP_SEQUENCER_LEDS_REC] = false;
        }

        leds[STEP_SEQUENCER_LEDS_MEMORY] = false;

        for (uint8_t ledToSet = STEP_SEQUENCER_LEDS_C; ledToSet <= STEP_SEQUENCER_LEDS_C2; ledToSet++)
        {
            if (steps[currentStepIndex].gate)
            {
                leds[ledToSet] = (ledToSet == noteToLedLookup[step.note]);
            }
        }

        leds[STEP_SEQUENCER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        leds[STEP_SEQUENCER_LEDS_OCTAVE_UP] = step.octaveUp;
        leds[STEP_SEQUENCER_LEDS_ACCENT] = step.accent;
        leds[STEP_SEQUENCER_LEDS_SLIDE] = step.slide;
        leds[STEP_SEQUENCER_LEDS_BACK] = false;
        leds[STEP_SEQUENCER_LEDS_NEXT] = false;
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
            //DEBUG("LED " << (uint16_t)ledIndex << " state: " << (leds[ledIndex] ? 0x01 : 0x00));
            returnValue = returnValue | ((leds[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void SequencerBrain::ActivateCurrentStep()
    {
        tick = stepInterval;
        UpdateLeds();

        if (steps[currentStepIndex].gate)
        {
            gateCount = 0;
            gateOn = true;
        }
    }

    void SequencerBrain::OnPlayPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_PLAY)
        {
            mode = STEP_SEQUENCER_MODE_STOP;
        }
        else if (mode == STEP_SEQUENCER_MODE_STOP || mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            currentStepIndex = 0;
            ActivateCurrentStep();
            mode = STEP_SEQUENCER_MODE_PLAY;
        }
    }

    void SequencerBrain::OnRecordPressed()
    {
        mode = STEP_SEQUENCER_MODE_STEP_REC;
        currentStepIndex = 0;
    }

    void SequencerBrain::OnBackPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            if (currentStepIndex > 0)
            {
                currentStepIndex--;
            }
        }
    }

    void SequencerBrain::OnNextPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            if (currentStepIndex < (stepCount - 1))
            {
                currentStepIndex++;
            }
        }
    }

    void SequencerBrain::OnOctaveDownPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            steps[currentStepIndex].octaveDown = !steps[currentStepIndex].octaveDown;
        }
    }

    void SequencerBrain::OnOctaveUpPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            steps[currentStepIndex].octaveUp = !steps[currentStepIndex].octaveUp;
        }
    }

    void SequencerBrain::OnAccentPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            steps[currentStepIndex].accent = !steps[currentStepIndex].accent;
        }
    }

    void SequencerBrain::OnSlidePressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            steps[currentStepIndex].slide = !steps[currentStepIndex].slide;
        }
    }

    void SequencerBrain::OnNoteKeyPressed()
    {
        if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            uint8_t note = GetNoteFromKeyPressed(lastKeyPress);

            if (note != STEP_SEQUENCER_NOT_NOTE_KEY)
            {
                if (steps[currentStepIndex].note == note)
                {
                    steps[currentStepIndex].gate = !steps[currentStepIndex].gate;
                }
                else
                {
                    steps[currentStepIndex].gate = true;
                    steps[currentStepIndex].note = note;
                }
            }
        }
    }

    void SequencerBrain::CheckForKeyPressEvent()
    {
        if (lastKeyPress != STEP_SEQUENCER_NO_KEY_PRESS)
        {
            switch (lastKeyPress)
            {
            case STEP_SEQUENCER_KEYS_PLAY:
                OnPlayPressed();
                break;
            case STEP_SEQUENCER_KEYS_REC:
                OnRecordPressed();
                break;
            case STEP_SEQUENCER_KEYS_BACK:
                OnBackPressed();
                break;
            case STEP_SEQUENCER_KEYS_NEXT:
                OnNextPressed();
                break;
            case STEP_SEQUENCER_KEYS_OCTAVE_DOWN:
                OnOctaveDownPressed();
                break;
            case STEP_SEQUENCER_KEYS_OCTAVE_UP:
                OnOctaveUpPressed();
                break;
            case STEP_SEQUENCER_KEYS_ACCENT:
                OnAccentPressed();
                break;
            case STEP_SEQUENCER_KEYS_SLIDE:
                OnSlidePressed();
                break;
            case STEP_SEQUENCER_KEYS_C:
            case STEP_SEQUENCER_KEYS_C_SHARP:
            case STEP_SEQUENCER_KEYS_D:
            case STEP_SEQUENCER_KEYS_D_SHARP:
            case STEP_SEQUENCER_KEYS_E:
            case STEP_SEQUENCER_KEYS_F:
            case STEP_SEQUENCER_KEYS_F_SHARP:
            case STEP_SEQUENCER_KEYS_G:
            case STEP_SEQUENCER_KEYS_G_SHARP:
            case STEP_SEQUENCER_KEYS_A:
            case STEP_SEQUENCER_KEYS_A_SHARP:
            case STEP_SEQUENCER_KEYS_B:
            case STEP_SEQUENCER_KEYS_C2:
                OnNoteKeyPressed();
                break;
            }

            lastKeyPress = STEP_SEQUENCER_NO_KEY_PRESS;
            UpdateLeds();
        }
    }

    void SequencerBrain::CheckForClockEvent()
    {
        if (mode == STEP_SEQUENCER_MODE_PLAY && tick <= 0)
        {
            currentStepIndex = (currentStepIndex + 1) % stepCount;
            ActivateCurrentStep();
        }

        if (tick <= (stepInterval - gateLength))
        {
            if (gateOn)
            {
                if (!steps[currentStepIndex].slide)
                {
                    gateOn = false;
                }

                if (steps[currentStepIndex].slide && mode != STEP_SEQUENCER_MODE_PLAY)
                {
                    gateOn = false;
                }
            }
        }

        if (tick > 0)
        {
            tick--;
        }
    }

    void SequencerBrain::Process(uint32_t currentProcessTimeUs)
    {
        CheckForKeyPressEvent();
        CheckForClockEvent();
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

    uint8_t SequencerBrain::GetNote()
    {
        uint8_t note = steps[currentStepIndex].note + 60;

        if (steps[currentStepIndex].octaveDown)
        {
            note -= 12;
        }

        if (steps[currentStepIndex].octaveUp)
        {
            note += 12;
        }

        return note;
    }

    void SequencerBrain::SetStepInterval(uint8_t newStepInterval)
    {
        stepInterval = newStepInterval;
        gateLength = stepInterval / 2;
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

        UpdateLeds();
    }

    uint8_t SequencerBrain::GetCurrentStepIndex()
    {
        return currentStepIndex;
    }

    uint8_t SequencerBrain::GetMode()
    {
        return mode;
    }

    Step SequencerBrain::GetCurrentStep()
    {
        return steps[currentStepIndex];
    }

    Step *SequencerBrain::GetSteps()
    {
        return steps;
    }
}