#pragma once
#ifndef POTENTIOMETER_ARRAY
#define POTENTIOMETER_ARRAY

#define POTENTIOMETER_COUNT 16
#define MUX1_COM_PIN 18
#define MUX2_COM_PIN 17
#define MUX_S0_PIN 19
#define MUX_S1_PIN 20
#define MUX_S2_PIN 21

#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::hardware::potentiometerArray::drivers
{
    using namespace daisysp;
    using namespace daisy;

    class PotentiometerArray
    {
    public:
        void Init();
        void Process();
        void Poll();
        DaisySeed *seed;
        AnalogControl analogControl[POTENTIOMETER_COUNT];
        float *GetValues();

    private:
        const uint8_t inputToIndexMapping[2][8] = {{1, 2, 3, 0, 4, 7, 5, 6}, {10, 9, 8, 11, 12, 15, 13, 14}};
        uint16_t rawReading[POTENTIOMETER_COUNT];
        AdcChannelConfig adcConfig[2];
    };
}

#endif