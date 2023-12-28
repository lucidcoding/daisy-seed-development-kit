#pragma once
#ifndef STEP_SEQUENCER_ILEDS_H
#define STEP_SEQUENCER_ILEDS_H

#include "daisysp.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;

    class ILeds
    {
    public:
        virtual void Init()= 0;
        virtual void SetLed(uint8_t ledIndex, bool state) = 0;
        virtual void Process() = 0;

    private:
    };
}

#endif