#include "FloatToIntScaler.h"
#include <math.h>
#include <stdexcept>

namespace developmentKit::library::utilities::scaling
{
    void FloatToIntScaler::Init(int16_t prmMin, int16_t prmMax, Curve prmCurve)
    {
        min = prmMin;
        max = prmMax;
        curve = prmCurve;
    }

    int16_t FloatToIntScaler::Scale(float in)
    {
        float floatValue;

        switch (curve)
        {
        case LINEAR:
            floatValue = (in * (max + 1 - min));
            break;
        case EXPONENTIAL:
            // WARNING - not yet tested!
            floatValue = in;
            floatValue = ((floatValue * floatValue) * (max + 1 - min));
            break;
        case LOGARITHMIC:
            // WARNING - not yet implemented!
            break;
        case CUBE:
            // WARNING - not yet tested!
            floatValue = in;
            floatValue = ((floatValue * (floatValue * floatValue)) * (max + 1 - min));
            break;
        default:
            break;
        }

        return (int16_t)floatValue + min;
    }

    float FloatToIntScaler::GetDivision()
    {
        return 1.0f / (float)(max - min + 1);
    }
}