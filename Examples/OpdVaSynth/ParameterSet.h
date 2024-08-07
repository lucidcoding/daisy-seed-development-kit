#pragma once
#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include <stdint.h>
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"

using namespace developmentKit::library::uiFramework::presenters;

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
    uint8_t midiChannel;
    PotentiometerArrayPageItem::KnobMode knobMode;

private:
};

#endif