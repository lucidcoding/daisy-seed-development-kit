#pragma once
#ifndef STEP_SEQUENCER_ISTATE_H
#define STEP_SEQUENCER_ISTATE_H

#include "stdint.h"
#include "Constants.h"
#include "IController.h"
#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class IState
    {
    public:
        void Init(IController *newController) { controller = newController; }
        virtual void Reset() = 0;
        virtual uint64_t GetLedState() = 0;
        virtual void CheckForClockEvent(uint32_t currentTicks) = 0;
        virtual void OnKeyPressed(uint32_t keyState) = 0;
        virtual void OnKeyReleased(uint32_t, uint32_t) = 0;
        virtual uint8_t GetStateCode() = 0;

    protected:
        IController *controller;

    private:
    };
}

#endif