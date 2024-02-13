#pragma once
#ifndef MOCK_UI_PARAMETER_PROVIDER_H
#define MOCK_UI_PARAMETER_PROVIDER_H

#include "../../Utilities/UiParameterProvider.h"

namespace developmentKit::library::uiFramework::tabs::tests
{
    using namespace developmentKit::library::uiFramework::tabs::utilities;

    class MockUiParameterProvider : public UiParameterProvider
    {
    public:
        MockUiParameterProvider(int prmMin, int prmMax, int prmValue)
        {
            value = prmValue;
            min = prmMin;
            max = prmMax;
        }
        ~MockUiParameterProvider() {}
        void SetValue(int prmValue) { value = prmValue; }
        void SetMin(int prmMin) { min = prmMin; }
        void SetMax(int prmMax) { max = prmMax; }
        int GetValue() { return value; }
        int GetMin() { return min; }
        int GetMax() { return max; }

    private:
        int value;
        int min;
        int max;
    };
}

#endif