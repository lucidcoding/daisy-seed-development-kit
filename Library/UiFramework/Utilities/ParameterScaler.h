#pragma once
#ifndef PARAMETER_SCALER_H
#define PARAMETER_SCALER_H

namespace developmentKit::library::uiFramework::tree::utilities
{
    class ParameterScaler
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
        void Init(float prmMin, float prmMax, Curve prmCurve);
        float Process(float rawFloat);

    private:
        float min, max;
        float lMin, lMax; 
        Curve curve;
        float value;
    };
}

#endif