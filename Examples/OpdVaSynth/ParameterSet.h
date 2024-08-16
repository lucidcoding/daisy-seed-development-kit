#pragma once
#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include <stdint.h>
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"
#include "vasynth.h"

using namespace developmentKit::library::uiFramework::presenters;

struct ParameterSet
{
public:
    uint8_t osc1WaveShape;
    float osc1Detune;
    uint8_t osc2WaveShape;
    float osc2Detune;
	float portamento;
    uint8_t voices;
    float osc2Transpose;
    float osc2Level;
    float pitchAttack;
    float pitchDecay;
    float pitchSustain;
    float pitchRelease;
    float noiseLevel;

    float filterAttack;
    float filterDecay;
    float filterSustain;
    float filterRelease; 
    float cutOff;
    float resonance;
    uint8_t filterType;
    float filterEnvelopeAmount;
    float ampAttack;
    float ampDecay;
    float ampSustain;
    float ampRelease;
    float pan;
    float mixLevel;

    float lfoLevel;
    float lfoFrequency;
    uint8_t lfoWaveShape;
    uint8_t lfoTarget;
    float reverbAmount;
    float reverbFeedback;
    float reverbFilter;
    float reverbDry;
    float reverbWet;
    float delayAmount;
    float delayFeedback;
    float delayTime;




    uint8_t note;

    uint8_t midiChannel;
    PotentiometerArrayPageItem::KnobMode knobMode;

private:
};

#endif