#include "BlinkState.h"
#include "Debug.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void BlinkState::Reset()
    {
        blinkJustStarted = true;
        blinkCount = 8;
        blinkOn = true;
    }

    uint64_t BlinkState::GetLedState()
    {
        uint64_t ledState = (uint64_t)0;

        if (blinkOn)
        {
            ledState = ledsToBlink;
        }

        return ledState;
    }

    void BlinkState::Process(uint32_t currentTicks, uint32_t keyState)
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
                controller->SwitchToStopState();
            }
        }
    }

    uint8_t BlinkState::GetStateCode()
    {
        return STEP_SEQUENCER_CONTROLLER_STATE_BLINK;
    }

    void BlinkState::SetLedsToBlink(uint64_t newLedsToBlink)
    {
        ledsToBlink = newLedsToBlink;
    }

    void BlinkState::SetTicksPerUs(uint32_t newTicksPerUs)
    {
        ticksPerUs = newTicksPerUs;
    }

    void BlinkState::SetBlinkTimeUs(uint32_t newBlinkTimeUs)
    {
        blinkTimeUs = newBlinkTimeUs;
    }
}