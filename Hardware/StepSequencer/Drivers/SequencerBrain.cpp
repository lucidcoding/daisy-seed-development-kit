#include "stdint.h"
#include "SequencerBrain.h"
#include "DEBUG.h"

namespace developmentKit::stepSequencer
{
    void SequencerBrain::Init()
    {
        currentStepIndex = 0;
        SetTicksPerStep(500);
        /*ticksPerStep = 500; // 500 is approx 120 bpm.
        ticksPerGate = ticksPerStep / 2;
        tickCountdown = ticksPerStep;*/
        mode = STEP_SEQUENCER_MODE_STOP;
        gate = false;

        for (uint8_t i = 0; i < 16; i++)
        {
            steps[i].note = 0;
            steps[i].gate = true;
            steps[i].octaveDown = false;
            steps[i].octaveUp = false;
            steps[i].accent = false;
            steps[i].slide = false;
        }

        UpdateLedStates();
    }

    void SequencerBrain::UpdateLedStates()
    {
        Step step = steps[currentStepIndex];

        for (uint8_t ledToSet = STEP_SEQUENCER_LEDS_C_SHARP; ledToSet <= STEP_SEQUENCER_LEDS_A_SHARP; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == noteToLedLookup[step.note]) && step.gate;
        }

        ledStates[STEP_SEQUENCER_LEDS_FUNC] = false;

        if (mode == STEP_SEQUENCER_MODE_PLAY)
        {
            ledStates[STEP_SEQUENCER_LEDS_PLAY] = true;
            ledStates[STEP_SEQUENCER_LEDS_REC] = false;
        }
        else if (mode == STEP_SEQUENCER_MODE_STEP_REC)
        {
            ledStates[STEP_SEQUENCER_LEDS_PLAY] = false;
            ledStates[STEP_SEQUENCER_LEDS_REC] = true;
        }
        else if (mode == STEP_SEQUENCER_MODE_STOP)
        {
            ledStates[STEP_SEQUENCER_LEDS_PLAY] = false;
            ledStates[STEP_SEQUENCER_LEDS_REC] = false;
        }

        ledStates[STEP_SEQUENCER_LEDS_MEMORY] = false;

        for (uint8_t ledToSet = STEP_SEQUENCER_LEDS_C; ledToSet <= STEP_SEQUENCER_LEDS_C2; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == noteToLedLookup[step.note]) && step.gate;
        }

        ledStates[STEP_SEQUENCER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        ledStates[STEP_SEQUENCER_LEDS_OCTAVE_UP] = step.octaveUp;
        ledStates[STEP_SEQUENCER_LEDS_ACCENT] = step.accent;
        ledStates[STEP_SEQUENCER_LEDS_SLIDE] = step.slide;
        ledStates[STEP_SEQUENCER_LEDS_BACK] = false;
        ledStates[STEP_SEQUENCER_LEDS_NEXT] = false;
    }

    void SequencerBrain::SetLastKeyPress(uint8_t newLastKeyPress)
    {
        lastKeyPress = newLastKeyPress;
    }

    uint64_t SequencerBrain::GetLedStates()
    {
        uint64_t returnValue = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_NUMBER_OF_LEDS; ledIndex++)
        {
            returnValue = returnValue | ((ledStates[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void SequencerBrain::ActivateCurrentStep()
    {
        tickCountdown = ticksPerStep;
        UpdateLedStates();

        if (steps[currentStepIndex].gate)
        {
            gate = true;
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
            if (currentStepIndex < (STEP_SEQUENCER_DEFAULT_STEP_COUNT - 1))
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
            UpdateLedStates();
        }
    }

    void SequencerBrain::CheckForClockEvent()
    {
        if (mode == STEP_SEQUENCER_MODE_PLAY && tickCountdown <= 0)
        {
            currentStepIndex = (currentStepIndex + 1) % STEP_SEQUENCER_DEFAULT_STEP_COUNT;
            ActivateCurrentStep();
        }

        if (tickCountdown <= (ticksPerStep - ticksPerGate))
        {
            if (gate)
            {
                if (!steps[currentStepIndex].slide)
                {
                    gate = false;
                }

                if (steps[currentStepIndex].slide && mode != STEP_SEQUENCER_MODE_PLAY)
                {
                    gate = false;
                }
            }
        }

        if (tickCountdown > 0)
        {
            tickCountdown--;
        }
    }

    void SequencerBrain::Process()
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
        return gate;
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

    bool SequencerBrain::GetAccent()
    {
        return steps[currentStepIndex].accent;
    }

    bool SequencerBrain::GetSlide()
    {
        return steps[currentStepIndex].slide;
    }

    bool SequencerBrain::GetPreviousSlide()
    {
        return steps[(currentStepIndex - 1) % STEP_SEQUENCER_DEFAULT_STEP_COUNT].slide;
    }

    void SequencerBrain::SetTicksPerStep(uint16_t setTicksPerStep)
    {
        ticksPerStep = setTicksPerStep;
        ticksPerGate = ticksPerStep / 2;
        tickCountdown = ticksPerStep;
    }

    uint8_t SequencerBrain::GetCurrentStepIndex()
    {
        return currentStepIndex;
    }

    uint8_t SequencerBrain::GetMode()
    {
        return mode;
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

        UpdateLedStates();
    }

    Step *SequencerBrain::GetSteps()
    {
        return steps;
    }
}