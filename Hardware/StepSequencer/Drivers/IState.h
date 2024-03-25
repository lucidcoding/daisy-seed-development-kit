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
        virtual uint64_t GetLedState(Step[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT], uint8_t) = 0;
        virtual void CheckForClockEvent(uint32_t currentTicks) = 0;
        virtual void OnKeyPressed(uint32_t keyState) = 0;

    protected:
        IController *controller;

    private:
    };
}

#endif