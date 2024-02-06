#include "UiParameter.h"
#include <math.h>

namespace developmentKit::library::uiFramework::utilities
{
    void UiParameter::Init(UiParameterProvider *prmUiParameterProvider, float prmMin, float prmMax, Curve prmCurve)
    {
        uiParameterProvider = prmUiParameterProvider;
        min = prmMin;
        max = prmMax;
        curve = prmCurve;
        lMin = logf(min < 0.0000001f ? 0.0000001f : min);
        lMax = logf(max);
    }

    float UiParameter::Process()
    {
        float displayValue = (float)uiParameterProvider->GetValue();
        float displayMin = (float)uiParameterProvider->GetMin();
        float displayMax = (float)uiParameterProvider->GetMax();
        float valueFactor = (displayValue / (displayMax - displayMin)) - (displayMin / (displayMax - displayMin));

        switch (curve)
        {
        case LINEAR:
            value = (valueFactor * (max - min)) + min;
            break;
        case EXPONENTIAL:
            value = valueFactor;
            value = ((value * value) * (max - min)) + min;
            break;
        case LOGARITHMIC:
            value = expf((valueFactor * (lMax - lMin)) + lMin);
            break;
        case CUBE:
            value = valueFactor;
            value = ((value * (value * value)) * (max - min)) + min;
            break;
        default:
            break;
        }
        return value;
    }
}