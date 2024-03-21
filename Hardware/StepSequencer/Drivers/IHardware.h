#pragma once
#ifndef STEP_SEQUENCER_IHARDWARE_H
#define STEP_SEQUENCER_IHARDWARE_H

#include "Step.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class IHardware
    {
    public:
        virtual void SavePatterns(Step *patterns)= 0;

    private:
    };
}

#endif