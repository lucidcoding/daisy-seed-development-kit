#pragma once
#ifndef STEP_SEQUENCER_ICONTROLLER_H
#define STEP_SEQUENCER_ICONTROLLER_H

#include "stdint.h"
#include "Constants.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class IController
    {
    public:
        virtual void SetState(uint8_t) = 0;

    private:
    };
}

#endif