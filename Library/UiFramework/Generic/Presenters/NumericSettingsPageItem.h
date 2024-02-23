#pragma once
#ifndef NUMERIC_SETTINGS_PAGE_ITEM_H
#define NUMERIC_SETTINGS_PAGE_ITEM_H

#include <string>
#include <vector>
#include "SettingsPageItem.h"
#include "../Utilities/UiParameterProvider.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::tree::utilities;

    class NumericSettingsPageItem : public SettingsPageItem
    {
    public:
        NumericSettingsPageItem() {}
        ~NumericSettingsPageItem() {}
        void Init(string prmTitle, ListPage *prmParent, int16_t prmMin, int16_t prmMax, int16_t prmDefault);
        void Increment();
        void Decrement();
        string GetValueAsString();
        string GetTitle();
        PageItemType GetType();
        int16_t GetValue();
        int16_t GetMin();
        int16_t GetMax();
        float GetScaledValue();

    private:
        string title;
        int16_t value;
        int16_t min;
        int16_t max;
    };
}

#endif