#include "stdint.h"
#include "math.h"
#include "Controller.h"
#include "Debug.h"
#include "IHardware.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    Controller::Controller(BlinkState *blinkState,
                           StopState *stopState,
                           LoadState *loadState,
                           PlayState *playState,
                           SaveState *saveState,
                           StepRecState *stepRecState,
                           SetSeqSyncState *setSeqSyncState)
    {
        _blinkState = blinkState;
        _stopState = stopState;
        _loadState = loadState;
        _playState = playState;
        _saveState = saveState;
        _stepRecState = stepRecState;
        _setSeqSyncState = setSeqSyncState;
    }

    void Controller::Init(uint32_t newTicksPerUs)
    {
        _blinkState->Init(this);
        _stopState->Init(this);
        _loadState->Init(this);
        _playState->Init(this);
        _saveState->Init(this);
        _stepRecState->Init(this);
        _setSeqSyncState->Init(this);
        hardware = NULL;
        currentStepIndex = 0;
        _playState->SetTicksPerUs(newTicksPerUs);
        _playState->SetStepTimeUs(1000000);
        _blinkState->SetTicksPerUs(newTicksPerUs);
        _blinkState->SetBlinkTimeUs(100000);
        SwitchToStopState();
        gate = false;
        ClearSteps();
        UpdateLedStates();
        seqSyncSource = STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL;
        _playState->SetSeqSyncSource(seqSyncSource);
        _setSeqSyncState->SetSeqSyncSource(seqSyncSource);

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

    void Controller::EnterTestMode()
    {
        _playState->SetStepTimeUs(STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP);
        _blinkState->SetBlinkTimeUs(STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP);
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
            _playState->SetStepTimeUs(intUsPerStep);
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
        _setSeqSyncState->SetSeqSyncSource(seqSyncSource);
        _playState->SetSeqSyncSource(seqSyncSource);
    }

    void Controller::SwitchToBlinkState(uint64_t ledsToBlink)
    {
        _blinkState->SetLedsToBlink(ledsToBlink);
        state = _blinkState;
        state->Reset();
    }

    void Controller::SwitchToLoadState(IState *backgroundState)
    {
        _loadState->SetBackgroundState(backgroundState);
        state = _loadState;
        state->Reset();
    }

    void Controller::SwitchToPlayStateAndRestart(uint32_t currentTicks)
    {
        state = _playState;
        _playState->Start(currentTicks);
    }

    void Controller::SwitchToPlayStateAndContinue()
    {
        state = _playState;
    }

    void Controller::SwitchToStopState()
    {
        state = _stopState;
        state->Reset();
    }

    void Controller::SwitchToStepRecState()
    {
        state = _stepRecState;
        state->Reset();
    }

    void Controller::SwitchToSaveState()
    {
        state = _saveState;
        state->Reset();
    }

    void Controller::SwitchToSetSeqSyncState()
    {
        state = _setSeqSyncState;
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

        if (currentStepIndex == 0)
        {
            _loadState->StartPattern();
        }
    }

    void Controller::MoveToFirstStep()
    {
        currentStepIndex = 0;
    }

    void Controller::SyncPulse2ppqn()
    {
        _playState->SyncPulse2ppqn();
    }

    void Controller::Process(uint32_t currentTicks, uint32_t keyState)
    {
        _playState->ProcessInBackground(currentTicks);
        state->Process(currentTicks, keyState);
        UpdateLedStates();
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