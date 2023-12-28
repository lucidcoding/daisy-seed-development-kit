#pragma once
#ifndef STEP_SEQUENCER_IKEYS_H
#define STEP_SEQUENCER_IKEYS_H

#include "daisysp.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;

    class IKeys
    {
    public:
        virtual void Init() = 0;
        virtual uint8_t ScanNextColumn() = 0;

    private:
    };
}

#endif