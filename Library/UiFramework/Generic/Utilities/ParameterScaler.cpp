#include "ParameterScaler.h"
#include <math.h>

namespace developmentKit::library::uiFramework::tree::utilities
{
    void ParameterScaler::Init(float prmMin, float prmMax, Curve prmCurve)
    {
        min = prmMin;
        max = prmMax;
        curve = prmCurve;
        lMin = logf(min < 0.0000001f ? 0.0000001f : min);
        lMax = logf(max);
    }

    float ParameterScaler::Process(float rawFloat)
    {
        switch (curve)
        {
        case LINEAR:
            value = (rawFloat * (max - min)) + min;
            break;
        case EXPONENTIAL:
            value = rawFloat;
            value = ((value * value) * (max - min)) + min;
            break;
        case LOGARITHMIC:
            value = expf((rawFloat * (lMax - lMin)) + lMin);
            break;
        case CUBE:
            value = rawFloat;
            value = ((value * (value * value)) * (max - min)) + min;
            break;
        default:
            break;
        }
        return value;
    }
}