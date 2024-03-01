#pragma once
#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include <stdint.h>

namespace developmentKit::library::uiFramework::tests::unitTests
{
    struct ParameterSet
    {
    public:
        float level;
        uint8_t note;
        uint8_t waveform;
        float attack;
        float decay;
        float sustain;
        float release;

    private:
    };
}

#endif