#include "stdint.h"
#include "math.h"
#include "Controller.h"
#include "Debug.h"
#include "IHardware.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void Controller::Init(uint32_t newTicksPerUs)
    {
        blinkState.Init(this);
        stopState.Init(this);
        loadState.Init(this);
        playState.Init(this);
        saveState.Init(this);
        stepRecState.Init(this);
        setSeqSyncState.Init(this);
        hardware = NULL;
        currentStepIndex = 0;
        playState.SetTicksPerUs(newTicksPerUs);
        playState.SetStepTimeUs(1000000);
        blinkState.SetTicksPerUs(newTicksPerUs);
        blinkState.SetBlinkTimeUs(100000);
        SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
        gate = false;
        ClearSteps();
        UpdateLedStates();
        lastKeyState = STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS;
        seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL;

        for (uint8_t savedStepIndex = 0; savedStepIndex < 128; savedStepIndex++)
        {
            savedPatterns[savedStepIndex].note = 0;
            savedPatterns[savedStepIndex].gate = true;
            savedPatterns[savedStepIndex].octaveDown = false;
            savedPatterns[savedStepIndex].octaveUp = false;
            savedPatterns[savedStepIndex].accent = false;
            savedPatterns[savedStepIndex].slide = false;
        }
    }

    void  Controller::EnterTestMode()
    {
        playState.SetStepTimeUs(STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP);
        blinkState.SetBlinkTimeUs(STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP);
    }

    void Controller::SetState(uint8_t newMode)
    {
        mode = newMode;

        switch (newMode)
        {
        case STEP_SEQUENCER_CONTROLLER_MODE_STOP:
            state = &stopState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_PLAY:
            state = &playState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC:
            state = &stepRecState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC:
            state = &setSeqSyncState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_BLINK:
            state = &blinkState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_SAVE:
            state = &saveState;
            break;
        case STEP_SEQUENCER_CONTROLLER_MODE_LOAD:
            state = &loadState;
            break;
        default:
            state = &stopState;
            break;
        }

        state->Reset();
    }

    void Controller::SetHardware(IHardware *prmHardware)
    {
        hardware = prmHardware;
    }

    void Controller::ClearSteps()
    {
        for (uint8_t i = 0; i < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; i++)
        {
            steps[i].note = 0;
            steps[i].gate = true;
            steps[i].octaveDown = false;
            steps[i].octaveUp = false;
            steps[i].accent = false;
            steps[i].slide = false;
        }

        // Blink(0x1FFFF);
    }

    void Controller::UpdateLedStates()
    {
        ledState = state->GetLedState();
    }

    uint64_t Controller::GetLedState()
    {
        return ledState;
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
            playState.SetStepTimeUs(intUsPerStep);
        }
    }

    void Controller::ActivateCurrentStep()
    {
        if (steps[currentStepIndex].gate)
        {
            gate = true;
        }
    }

    void Controller::ToggleSeqSyncSource()
    {
        seqSyncSource = (seqSyncSource + 1) % 4;
        setSeqSyncState.SetSeqSyncSource(seqSyncSource);
    }

    void Controller::Blink(uint64_t ledsToBlink)
    {
        blinkState.SetLedsToBlink(ledsToBlink);
        state = &blinkState;
        mode = STEP_SEQUENCER_CONTROLLER_MODE_BLINK;
        state->Reset();
    }

    void Controller::MoveBackStep()
    {
        if (currentStepIndex > 0)
        {
            currentStepIndex--;
        }
    }

    void Controller::MoveNextStep()
    {
        currentStepIndex = (currentStepIndex + 1) % STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT;
    }

    void Controller::MoveToFirstStep()
    {
        currentStepIndex = 0;
    }

    void Controller::SetKeyState(uint32_t keyState)
    {
        if (keyState == STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            return;
        }

        if ((lastKeyState & keyState) == keyState)
        {
            state->OnKeyReleased(keyState, lastKeyState);
        }
        else
        {
            state->OnKeyPressed(keyState);
        }

        UpdateLedStates();
        lastKeyState = keyState;
    }

    void Controller::Process(uint32_t currentTicks)
    {
        state->CheckForClockEvent(currentTicks);
        UpdateLedStates();
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

    uint8_t Controller::GetNoteFromPatternIndex(uint8_t patternIndex)
    {
        uint8_t lookup[8] = {0, 2, 4, 5, 7, 9, 11, 12};
        return lookup[patternIndex];
    }

    uint8_t Controller::GetPatternIndexFromNote(uint8_t noteNumber)
    {
        uint8_t lookup[8] = {0, 2, 4, 5, 7, 9, 11, 12};

        for (uint8_t index = 0; index < 8; index++)
        {
            if (lookup[index] == noteNumber)
            {
                return index;
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

    void Controller::SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT])
    {
        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            steps[stepIndex].gate = newSteps[stepIndex].gate;
            steps[stepIndex].note = newSteps[stepIndex].note;
            steps[stepIndex].octaveDown = newSteps[stepIndex].octaveDown;
            steps[stepIndex].octaveUp = newSteps[stepIndex].octaveUp;
            steps[stepIndex].accent = newSteps[stepIndex].accent;
            steps[stepIndex].slide = newSteps[stepIndex].slide;
        }
    }

    void Controller::SavePattern(uint8_t patternIndex)
    {
        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            uint8_t savedPatternIndex = (patternIndex * STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT) + stepIndex;
            savedPatterns[savedPatternIndex].note = steps[stepIndex].note;
            savedPatterns[savedPatternIndex].gate = steps[stepIndex].gate;
            savedPatterns[savedPatternIndex].octaveDown = steps[stepIndex].octaveDown;
            savedPatterns[savedPatternIndex].octaveUp = steps[stepIndex].octaveUp;
            savedPatterns[savedPatternIndex].accent = steps[stepIndex].accent;
            savedPatterns[savedPatternIndex].slide = steps[stepIndex].slide;
        }

        hardware->SavePatterns(savedPatterns);
    }

    void Controller::LoadPattern(uint8_t patternIndex)
    {
        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            uint8_t savedPatternIndex = (patternIndex * STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT) + stepIndex;
            steps[stepIndex].note = savedPatterns[savedPatternIndex].note;
            steps[stepIndex].gate = savedPatterns[savedPatternIndex].gate;
            steps[stepIndex].octaveDown = savedPatterns[savedPatternIndex].octaveDown;
            steps[stepIndex].octaveUp = savedPatterns[savedPatternIndex].octaveUp;
            steps[stepIndex].accent = savedPatterns[savedPatternIndex].accent;
            steps[stepIndex].slide = savedPatterns[savedPatternIndex].slide;
        }
    }
}