#include "BlinkState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    uint64_t BlinkState::GetLedState(Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t currentStepIndex)
    {
        uint64_t ledState = (uint64_t)0;

        if(blinkOn) 
        {
            ledState = ledsToBlink;
        } 
        
        return ledState;
    }

    void BlinkState::CheckForClockEvent(uint32_t currentTicks)
    {
        if (blinkJustStarted)
        {
            blinkJustStarted = false;
            lastBlinkTicks = currentTicks - 1;
        }

        if ((currentTicks - lastBlinkTicks) >= (blinkTimeUs * ticksPerUs))
        {
            lastBlinkTicks = currentTicks;
            blinkOn = !blinkOn;

            if (blinkCount-- == 0)
            {
                controller->SetState(STEP_SEQUENCER_CONTROLLER_MODE_STOP);
            }
        }
    }

    void BlinkState::SetLedsToBlink(uint64_t newLedsToBlink)
    {
        ledsToBlink = newLedsToBlink;
    }

    void BlinkState::SetTicksPerUs(uint32_t newTicksPerUs)
    {
        ticksPerUs = newTicksPerUs;
    }

    void BlinkState::StartBlink()
    {
        blinkJustStarted = true;
        blinkCount = 8;
        blinkOn = true;
    }

    void BlinkState::SetBlinkTimeUs(uint32_t newBlinkTimeUs)
    {
        blinkTimeUs = newBlinkTimeUs;
    }
}