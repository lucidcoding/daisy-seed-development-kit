#include "Debug.h"
#include "PlayState.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void PlayState::Reset()
    {
        controller->MoveToFirstStep();
        controller->ActivateCurrentStep();
        external2PpqnPulseOn = false;
        ticksBetweenPulses = 0;
        lastPulseTicks = 0;
        intermediaryPulseDone = false;
    }

    void PlayState::Start(uint32_t currentTicks)
    {
        lastStepStartTicks = currentTicks;
        lastPulseTicks = currentTicks;
        controller->MoveToFirstStep();
        controller->ActivateCurrentStep();
        external2PpqnPulseOn = false;
        ticksBetweenPulses = 0;
        lastPulseTicks = 0;
        playIntermediaryNote = false;
        firstPulseReceived = false;
    }

    /*void PlayState::Continue(uint32_t currentTicks)
    {
        lastStepStartTicks = currentTicks;
        lastPulseTicks = currentTicks;
        controller->MoveToFirstStep();
        controller->ActivateCurrentStep();
        pulseOn = false;
        ticksBetweenPulses = 0;
        lastPulseTicks = 0;
        playIntermediaryNote = false;
        firstPulseReceived = false;
    }*/

    uint64_t PlayState::GetLedState()
    {
        Step *steps = controller->GetSteps();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        uint64_t ledState = (uint64_t)0;
        ledState |= (uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY;
        ledState |= stepIndicator.GetLedState(steps, currentStepIndex);
        return ledState;
    }

    void PlayState::ProcessInBackground(uint32_t currentTicks)
    {
        if (external2PpqnPulseOn)
        {
            external2PpqnPulseOn = false;
            DEBUG("Pulse ON");
            ticksBetweenPulses = currentTicks - lastExternal2PpqnPulseTicks;
            lastExternal2PpqnPulseTicks = currentTicks;
            intermediaryPulseDone = false;
            DEBUG("ticksBetweenPulses: %lu", ticksBetweenPulses);
        }

        if (!intermediaryPulseDone && (currentTicks - lastExternal2PpqnPulseTicks) >= (ticksBetweenPulses / 2))
        {
            DEBUG("Intermediary pulse ON");
            DEBUG("currentTicks: %lu, lastExternal2PpqnPulseTicks: %lu, ticksBetweenPulses / 2: %lu", currentTicks, lastExternal2PpqnPulseTicks, ticksBetweenPulses / 2);
            intermediaryPulseDone = true;
        }
    }

    void PlayState::Process(uint32_t currentTicks, uint32_t keyState)
    {
        if (keyState != STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS)
        {
            OnKeyPressed(keyState);
        }

        bool gate = controller->GetGate();
        uint8_t currentStepIndex = controller->GetCurrentStepIndex();
        Step *steps = controller->GetSteps();

        if (gate && (currentTicks - lastStepStartTicks) >= (gateTimeUs * ticksPerUs))
        {
            if (!steps[currentStepIndex].slide)
            {
                gate = false;
                controller->SetGate(gate);
            }
        }

        if (seqSyncSource == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL)
        {
            if ((currentTicks - lastStepStartTicks) >= (stepTimeUs * ticksPerUs))
            {
                lastStepStartTicks = currentTicks;
                controller->MoveNextStep();
                controller->ActivateCurrentStep();
            }
        }

        /*if (seqSyncSource == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE)
        {
            if (playIntermediaryNote && currentTicks >= playIntermediateNoteTicks)
            {
                controller->MoveNextStep();
                controller->ActivateCurrentStep();
                playIntermediaryNote = false;
            }

            if (external2PpqnPulseOn)
            {
                external2PpqnPulseOn = false;
                ticksBetweenPulses = currentTicks - lastPulseTicks;
                playIntermediateNoteTicks = (currentTicks + (ticksBetweenPulses / 2));

                if (firstPulseReceived)
                {
                    playIntermediaryNote = true;
                }

                lastPulseTicks = currentTicks;
                lastStepStartTicks = currentTicks;
                firstPulseReceived = true;
                controller->MoveNextStep();
                controller->ActivateCurrentStep();
            }
        }*/
    }

    void PlayState::OnKeyPressed(uint32_t keyState)
    {
        switch (keyState)
        {
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnSavePatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP):
            OnSeqSyncSelectPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN):
            OnLoadPatternPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2):
            OnClearPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC):
            OnRecordPressed();
            break;
        case (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY):
            OnPlayPressed();
            break;
        }
    }

    /*void PlayState::OnKeyReleased(uint32_t keyState, uint32_t lastKeyState)
    {
    }*/

    uint8_t PlayState::GetStateCode()
    {
        return STEP_SEQUENCER_CONTROLLER_MODE_PLAY;
    }

    void PlayState::OnSavePatternPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    }

    void PlayState::OnSeqSyncSelectPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    }

    void PlayState::OnLoadPatternPressed()
    {
        controller->SwitchToLoadState(this);
    }

    void PlayState::OnClearPressed()
    {
        controller->ClearSteps();
        controller->SwitchToBlinkState(0x1FFFF);
    }

    void PlayState::OnRecordPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
    }

    void PlayState::OnPlayPressed()
    {
        controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
        controller->SetGate(false);
    }

    void PlayState::SetStepTimeUs(uint32_t newStepTimeUs)
    {
        stepTimeUs = newStepTimeUs;
        gateTimeUs = stepTimeUs / 2;
    }

    void PlayState::SetTicksPerUs(uint32_t newTicksPerUs)
    {
        ticksPerUs = newTicksPerUs;
    }

    void PlayState::SyncPulse2ppqn()
    {
        external2PpqnPulseOn = true;
    }

    void PlayState::SetSeqSyncSource(uint8_t newSeqSyncSource)
    {
        seqSyncSource = newSeqSyncSource;
    }
}