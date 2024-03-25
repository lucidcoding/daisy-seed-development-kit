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
        playState.Init(this);
        saveState.Init(this);
        stepRecState.Init(this);
        setSeqSyncState.Init(this);

        ticksPerUs = newTicksPerUs;
        hardware = NULL;
        currentStepIndex = 0;
        SetStepTime(1000000);
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
    }

    void Controller::UpdateLedStates()
    {
        Step step = steps[currentStepIndex];
        ledState = state->GetLedState();
        return;
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

    void Controller::OnSavePatternPressed()
    {
        SetState(STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    }

    void Controller::OnSeqSyncSelectPressed()
    {
        if (mode != STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC)
        {
            SetState(STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
        }
        else
        {
            switch (seqSyncSource)
            {
            case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL:
                seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE;
                break;
            case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE:
                seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SYNC;
                break;
            case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SYNC:
                seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SEQ;
                break;
            case STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SEQ:
                seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL;
                break;
            default:
                seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL;
            };
        }

        setSeqSyncState.SetSeqSyncSource(seqSyncSource);
    }

    void Controller::OnClearPressed()
    {
        ClearSteps();
        SetState(STEP_SEQUENCER_CONTROLLER_MODE_BLINK);
        blinkState.SetLedsToBlink(0x1FFFF);
        UpdateLedStates();
    }

    void Controller::OnRecordPressed()
    {
        SetState(STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
        currentStepIndex = 0;
    }

    void Controller::OnNoteKeyPressed(uint64_t keyState)
    {
        uint8_t note = GetNoteFromKeyPressed(keyState);

        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_SAVE)
        {
            uint8_t patternIndex = GetPatternIndexFromNote(note);
            SavePattern(patternIndex);
            SetState(STEP_SEQUENCER_CONTROLLER_MODE_BLINK);
            savingLed = note;
            blinkState.SetLedsToBlink((uint64_t)1 << savingLed);
            blinkState.Reset();
            UpdateLedStates();
        }
    }

    void Controller::OnFunctionKeyReleased()
    {
        if (mode == STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC)
        {
            SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
        }
    }

    void Controller::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnSavePatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnClearPressed();
            break;
        /*case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_E):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_G):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_A):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_B):
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnSelectPatternPressed(keyState);
            break;*/
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            //OnPlayPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC):
            OnRecordPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_BACK):
            //OnBackPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT):
            //OnNextPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN):
            //OnOctaveDownPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP):
            //OnOctaveUpPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT):
            //OnAccentPressed();
            state->OnKeyPressed(keyState);
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE):
            //OnSlidePressed();
            state->OnKeyPressed(keyState);
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
            state->OnKeyPressed(keyState);
            break;
        }
    }

    void Controller::OnKeyReleased(uint32_t keyState)
    {
        if (((lastKeyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) > 0) && ((keyState & ((uint32_t)1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC)) == 0))
        {
            OnFunctionKeyReleased();
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

    uint8_t Controller::GetPatternIndexFromNote(uint8_t noteNumber)
    {
        uint8_t lookup[8] = {0, 2, 4, 6, 7, 9, 11, 12};

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

    void Controller::SetStepTime(uint32_t newStepTimeUs)
    {
        stepTimeUs = newStepTimeUs;
        gateTimeUs = stepTimeUs / 2;
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

        UpdateLedStates();
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

    /*void Controller::LoadPattern(uint8_t patternIndex)
    {
        for (uint8_t stepIndex = 0; stepIndex < STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT; stepIndex++)
        {
            uint8_t savedPatternIndex = (patternIndex * STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT) + stepIndex;
            savedPatterns[savedPatternIndex].note = savedPatterns[stepIndex].note;
            steps[stepIndex].gate = savedPatterns[savedPatternIndex].gate;
            steps[stepIndex].octaveDown = savedPatterns[savedPatternIndex].octaveDown;
            steps[stepIndex].octaveUp = savedPatterns[savedPatternIndex].octaveUp;
            steps[stepIndex].accent = savedPatterns[savedPatternIndex].accent;
            steps[stepIndex].slide = savedPatterns[savedPatternIndex].slide;
        }
    }*/
}