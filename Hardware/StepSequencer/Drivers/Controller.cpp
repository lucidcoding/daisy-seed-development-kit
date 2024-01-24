#include "stdint.h"
#include "Controller.h"
#include "DEBUG.h"

namespace developmentKit::stepSequencer
{
    void Controller::Init()
    {
        currentStepIndex = 0;
        SetTicksPerStep(500);
        mode = STEP_SEQUENCER_CONTROLLER_MODE_STOP;
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

    void Controller::UpdateLedStates()
    {
        Step step = steps[currentStepIndex];

        for (uint8_t ledToSet = STEP_SEQUENCER_CONTROLLER_LEDS_C_SHARP; ledToSet <= STEP_SEQUENCER_CONTROLLER_LEDS_A_SHARP; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == noteToLedLookup[step.note]) && step.gate;
        }

        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_FUNC] = false;

        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_PLAY)
        {
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_PLAY] = true;
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_REC] = false;
        }
        else if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_PLAY] = false;
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_REC] = true;
        }
        else if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STOP)
        {
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_PLAY] = false;
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_REC] = false;
        }

        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_MEMORY] = false;

        for (uint8_t ledToSet = STEP_SEQUENCER_CONTROLLER_LEDS_C; ledToSet <= STEP_SEQUENCER_CONTROLLER_LEDS_C2; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == noteToLedLookup[step.note]) && step.gate;
        }

        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP] = step.octaveUp;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT] = step.accent;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE] = step.slide;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_BACK] = false;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_NEXT] = false;
    }

    void Controller::SetKeyState(uint32_t newLastKeyPress)
    {
        keyState = newLastKeyPress;
    }

    uint64_t Controller::GetLedState()
    {
        uint64_t returnValue = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS; ledIndex++)
        {
            returnValue = returnValue | ((ledStates[ledIndex] ? 0x01 : 0x00) << ledIndex);
        }

        return returnValue;
    }

    void Controller::SetTempo(uint8_t newTempo)
    {
        SetTicksPerStep(500 - newTempo);
    }

    void Controller::ActivateCurrentStep()
    {
        tickCountdown = ticksPerStep;
        UpdateLedStates();

        if (steps[currentStepIndex].gate)
        {
            gate = true;
        }
    }

    void Controller::OnPlayPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_PLAY)
        {
            mode = STEP_SEQUENCER_CONTROLLER_MODE_STOP;
        }
        else if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STOP || mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            currentStepIndex = 0;
            ActivateCurrentStep();
            mode = STEP_SEQUENCER_CONTROLLER_MODE_PLAY;
        }
    }

    void Controller::OnRecordPressed()
    {
        mode = STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC;
        currentStepIndex = 0;
    }

    void Controller::OnBackPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            if (currentStepIndex > 0)
            {
                currentStepIndex--;
            }
        }
    }

    void Controller::OnNextPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            if (currentStepIndex < (STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT - 1))
            {
                currentStepIndex++;
            }
        }
    }

    void Controller::OnOctaveDownPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            steps[currentStepIndex].octaveDown = !steps[currentStepIndex].octaveDown;
        }
    }

    void Controller::OnOctaveUpPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            steps[currentStepIndex].octaveUp = !steps[currentStepIndex].octaveUp;
        }
    }

    void Controller::OnAccentPressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            steps[currentStepIndex].accent = !steps[currentStepIndex].accent;
        }
    }

    void Controller::OnSlidePressed()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            steps[currentStepIndex].slide = !steps[currentStepIndex].slide;
        }
    }

    void Controller::OnNoteKeyPressed(uint8_t keyIndex)
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            uint8_t note = GetNoteFromKeyPressed(keyIndex);

            if (note != STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY)
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

    void Controller::CheckForKeyPressEvent()
    {
        if (keyState != STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            for (uint8_t keyIndex = 0; keyIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_KEYS; keyIndex++)
            {
                bool keyIsPressed = (keyState & (1 << keyIndex)) > 0;

                if (keyIsPressed)
                {
                    switch (keyIndex)
                    {
                    case STEP_SEQUENCER_CONTROLLER_KEYS_PLAY:
                        OnPlayPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_REC:
                        OnRecordPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_BACK:
                        OnBackPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_NEXT:
                        OnNextPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN:
                        OnOctaveDownPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP:
                        OnOctaveUpPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT:
                        OnAccentPressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE:
                        OnSlidePressed();
                        break;
                    case STEP_SEQUENCER_CONTROLLER_KEYS_C:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_D:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_E:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_F:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_G:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_A:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_B:
                    case STEP_SEQUENCER_CONTROLLER_KEYS_C2:
                        OnNoteKeyPressed(keyIndex);
                        break;
                    }

                    keyState = keyState && ~(1 << keyIndex);
                    UpdateLedStates();
                }
            }
        }
    }

    void Controller::CheckForClockEvent()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_PLAY && tickCountdown <= 0)
        {
            currentStepIndex = (currentStepIndex + 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
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

                if (steps[currentStepIndex].slide && mode != STEP_SEQUENCER_CONTROLLER_MODE_PLAY)
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

    void Controller::Process()
    {
        CheckForKeyPressEvent();
        CheckForClockEvent();
    }

    uint8_t Controller::GetNoteFromKeyPressed(uint8_t keyPressed)
    {
        for (uint8_t currentIndex = 0; currentIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_NOTE_KEYS; currentIndex++)
        {
            if (noteToLedLookup[currentIndex] == keyPressed)
            {
                return currentIndex;
            }
        }

        return STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY;
    }

    bool Controller::GetGate()
    {
        return gate;
    }

    uint8_t Controller::GetNote()
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

    bool Controller::GetAccent()
    {
        return steps[currentStepIndex].accent;
    }

    bool Controller::GetSlide()
    {
        return steps[currentStepIndex].slide;
    }

    bool Controller::GetPreviousSlide()
    {
        uint8_t previousStepIndex = (currentStepIndex - 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
        return steps[previousStepIndex].slide;
    }

    void Controller::SetTicksPerStep(uint16_t setTicksPerStep)
    {
        ticksPerStep = setTicksPerStep;
        ticksPerGate = ticksPerStep / 2;
        tickCountdown = ticksPerStep;
    }

    uint8_t Controller::GetCurrentStepIndex()
    {
        return currentStepIndex;
    }

    uint8_t Controller::GetMode()
    {
        return mode;
    }

    void Controller::SetSteps(Step newSteps[16])
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

    Step *Controller::GetSteps()
    {
        return steps;
    }
}