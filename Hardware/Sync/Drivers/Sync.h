#pragma once
#ifndef SYNC_H
#define SYNC_H

#define SYNC_PULSE_LENGTH_MS 15

#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::hardware::sync::drivers
{
    using namespace daisysp;
    using namespace daisy;

    class Sync
    {
    public:
        void Init(DaisySeed *);
        bool Poll();

    private:
        dsy_gpio syncInput;
        dsy_gpio syncOutput;
        bool lastInputReading;
    };
}

#endif