#pragma once
#ifndef FLOAT_TO_INT_SCALER_H
#define FLOAT_TO_INT_SCALER_H

#include <string>

using namespace std;

namespace developmentKit::library::utilities::scaling
{
    class FloatToIntScaler
    {
    public:
        enum Curve
        {
            LINEAR,
            EXPONENTIAL,
            LOGARITHMIC,
            CUBE,
            LAST,
        };
        FloatToIntScaler() {}
        ~FloatToIntScaler() {}
        void Init(int16_t prmMin, int16_t prmMax, Curve prmCurve);
        int16_t Scale(float in);
        float GetDivision();

    private:
        int16_t min;
        int16_t max;
        Curve curve;
    };
}

#endif