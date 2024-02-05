#include "UiParameter.h"
#include "../Presenters/NumericSettingsPageItem.h"
#include <math.h>

namespace developmentKit::library::uiFramework::utilities
{
    using namespace developmentKit::library::uiFramework::presenters;

    void UiParameter::Init(NumericSettingsPageItem *prmNumericSettingsPageItem, float prmMin, float prmMax, Curve prmCurve)
    {
        numericSettingsPageItem = prmNumericSettingsPageItem;
        min = prmMin;
        max = prmMax;
        curve = prmCurve;
        lMin = logf(min < 0.0000001f ? 0.0000001f : min);
        lMax = logf(max);
    }

    float UiParameter::Process()
    {
        float displayValue = (float)numericSettingsPageItem->GetValue();
        float displayMin = (float)numericSettingsPageItem->GetMin();
        float displayMax = (float)numericSettingsPageItem->GetMax();
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