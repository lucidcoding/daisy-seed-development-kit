#pragma once
#ifndef POTENTIOMETER_TO_DISPLAY_VALUE_SCALER_H
#define POTENTIOMETER_TO_DISPLAY_VALUE_SCALER_H

#include <stdint.h>

namespace developmentKit::library::uiFramework::shared::scaling
{
    class PotentiometerToDisplayValueScaler
    {
    public:
        PotentiometerToDisplayValueScaler() {}
        ~PotentiometerToDisplayValueScaler() {}
        void Init(int16_t prmMin, int16_t prmMax);
        int16_t Scale(float in);
        float GetDivision();

    private:
        int16_t min;
        int16_t max;
    };
}

#endif