#pragma once
#ifndef NUMERIC_SETTINGS_PAGE_ITEM_H
#define NUMERIC_SETTINGS_PAGE_ITEM_H

#include <vector>
#include <stdint.h>
#include "SettingsListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    class NumericSettingsListPageItem : public SettingsListPageItem
    {
    public:
        NumericSettingsListPageItem() {}
        ~NumericSettingsListPageItem() {}
        void Init(const char *prmTitle, ListPage *prmParent, int16_t prmMin, int16_t prmMax, int16_t prmDefault);
        void Increment();
        void Decrement();
        const char * GetValueAsString();
        const char *GetTitle();
        ListPageItemType GetType();
        int16_t GetValue();
        int16_t GetMin();
        int16_t GetMax();
        float GetScaledValue();

    private:
        const char *title;
        int16_t value;
        int16_t min;
        int16_t max;
        char displayString[16];
    };
}

#endif