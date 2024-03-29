#pragma once
#ifndef UI_PARAMETER_H
#define UI_PARAMETER_H

#include <string>
#include <vector>
#include "../Presenters/NumericSettingsPageItem.h"

namespace developmentKit::library::uiFramework::utilities
{
    class UiParameter
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
        void Init(UiParameterProvider *prmUiParameterProvider, float prmMin, float prmMax, Curve prmCurve);
        float Process();

    private:
        UiParameterProvider *uiParameterProvider;
        float min, max;
        float lMin, lMax; 
        Curve curve;
        float value;
    };
}

#endif