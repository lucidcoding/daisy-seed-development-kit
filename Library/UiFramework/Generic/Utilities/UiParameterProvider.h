#pragma once
#ifndef UI_PARAMETER_PROVIDER_H
#define UI_PARAMETER_PROVIDER_H

#include <vector>

namespace developmentKit::library::uiFramework::tree::utilities
{
    class UiParameterProvider
    {
    public:
        UiParameterProvider() {}
        ~UiParameterProvider() {}
        virtual int GetMin() = 0;
        virtual int GetMax() = 0;
        virtual int GetValue() = 0;

    private:
    };
}

#endif