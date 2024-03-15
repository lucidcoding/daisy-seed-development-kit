#include "stdint.h"
#include "math.h"
#include "Controller.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void Controller::Init(uint32_t newTicksPerUs)
    {
        ticksPerUs = newTicksPerUs;
        currentStepIndex = 0;
        SetStepTime(1000000);
        mode = STEP_SEQUENCER_CONTROLLER_MODE_STOP;
        gate = false;
        playJustPressed = false;

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
        lastKeyState = STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS;
    }

    void Controller::UpdateLedStates()
    {
        Step step = steps[currentStepIndex];

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

        for (uint8_t ledToSet = STEP_SEQUENCER_CONTROLLER_LEDS_C; ledToSet <= STEP_SEQUENCER_CONTROLLER_LEDS_C2; ledToSet++)
        {
            ledStates[ledToSet] = (ledToSet == step.note) && step.gate;
        }

        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_DOWN] = step.octaveDown;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP] = step.octaveUp;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT] = step.accent;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE] = step.slide;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_BACK] = false;
        ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_NEXT] = false;

        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            ledStates[STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + stepIndex] = (stepIndex == currentStepIndex);
        }
    }

    uint64_t Controller::GetLedState()
    {
        uint64_t returnValue = 0x00;

        for (uint8_t ledIndex = 0; ledIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS; ledIndex++)
        {
            returnValue = returnValue | ((ledStates[ledIndex] ? (uint64_t)1 : (uint64_t)0) << ledIndex);
        }

        return returnValue;
    }

    void Controller::SetTempo(uint8_t newTempo)
    {
        if (newTempo != tempo)
        {
            tempo = newTempo;
            uint16_t stepsPerMinute = newTempo * 4;
            float stepsPerSecond = stepsPerMinute / 60;
            float stepsPerUs = stepsPerSecond / 1000000;
            float usPerStep = 1 / stepsPerUs;
            uint32_t intUsPerStep = (uint32_t)usPerStep;
            SetStepTime(intUsPerStep);
        }
    }

    void Controller::ActivateCurrentStep()
    {
        UpdateLedStates();

        if (steps[currentStepIndex].gate)
        {
            gate = true;
        }
    }

    void Controller::OnSeqSyncSelectPressed()
    {
        //daisy->PrintLine("OnSeqSyncSelectPressed");
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
            playJustPressed = true;
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

    void Controller::OnNoteKeyPressed(uint64_t keyState)
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC)
        {
            uint8_t note = GetNoteFromKeyPressed(keyState);

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

    void Controller::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC):
            OnRecordPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_BACK):
            OnBackPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT):
            OnNextPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN):
            OnOctaveDownPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP):
            OnOctaveUpPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT):
            OnAccentPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE):
            OnSlidePressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_E):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_B):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnNoteKeyPressed(keyState);
            break;
        }
    }

    void Controller::OnKeyReleased(uint32_t keyState)
    {
        
       //daisy->PrintLine("Release: %d, keyState: %d, lastKeyState: %d", keyState, lastKeyState);

        if(((lastKeyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) > 0) && ((keyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) == 0))
        {
            //daisy->PrintLine("FUNC released");
        }
    }

    void Controller::SetKeyState(uint32_t keyState)
    {
        if (keyState == STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            return;
        }

        if ((lastKeyState & keyState) == keyState)
        {
            OnKeyReleased(keyState);
        }
        else
        {
            OnKeyPressed(keyState);
        }

        UpdateLedStates();
        lastKeyState = keyState;
    }

    void Controller::CheckForClockEvent(uint32_t currentTicks)
    {
        if (playJustPressed)
        {
            playJustPressed = false;
            lastStepStartTicks = currentTicks;
        }

        if (gate && (currentTicks - lastStepStartTicks) >= (gateTimeUs * ticksPerUs))
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

        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_PLAY && (currentTicks - lastStepStartTicks) >= (stepTimeUs * ticksPerUs))
        {
            lastStepStartTicks = currentTicks;
            currentStepIndex = (currentStepIndex + 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
            ActivateCurrentStep();
        }
    }

    void Controller::Process(uint32_t currentProcessTimeUs)
    {
        CheckForClockEvent(currentProcessTimeUs);
    }

    uint8_t Controller::GetNoteFromKeyPressed(uint32_t keyState)
    {
        uint8_t keyPressed = STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY;

        for (uint8_t currentKeyIndex = 0; currentKeyIndex <= 12; currentKeyIndex++)
        {
            if ((keyState & (1 << currentKeyIndex)) > 0)
            {
                keyPressed = currentKeyIndex;
                break;
            }
        }

        for (uint8_t currentIndex = 0; currentIndex < STEP_SEQUENCER_CONTROLLER_NUMBER_OF_NOTE_KEYS; currentIndex++)
        {
            if (currentIndex == keyPressed)
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
        uint8_t note = steps[currentStepIndex].note + 36;

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
        uint8_t previousStepIndex = (currentStepIndex + STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT - 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
        return steps[previousStepIndex].slide;
    }

    void Controller::SetStepTime(uint32_t newStepTimeUs)
    {
        stepTimeUs = newStepTimeUs;
        gateTimeUs = stepTimeUs / 2;
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
}