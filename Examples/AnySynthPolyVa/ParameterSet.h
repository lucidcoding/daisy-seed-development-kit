#pragma once
#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include <stdint.h>
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"

using namespace developmentKit::library::uiFramework::presenters;

struct ParameterSet
{
public:
    uint8_t osc1WaveShape;
    float osc1Detune;
    uint8_t osc2WaveShape;
    float osc2Transpose;
    float osc2Detune;
    float osc2Level;
    float noiseLevel;
	float portamento;
    float mixLevel;
    float pan;

    float cutOff;
    float resonance;
    
    float level;
    uint8_t note;
    float attack;
    float decay;
    float sustain;
    float release;
    uint8_t midiChannel;
    PotentiometerArrayPageItem::KnobMode knobMode;

private:
};

#endif