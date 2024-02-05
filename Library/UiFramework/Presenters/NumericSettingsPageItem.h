#pragma once
#ifndef NUMERIC_SETTINGS_PAGE_ITEM_H
#define NUMERIC_SETTINGS_PAGE_ITEM_H

#include <string>
#include <vector>
#include "SettingsPageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class NumericSettingsPageItem : public SettingsPageItem
    {
    public:
        NumericSettingsPageItem(string prmTitle, ListPage *prmParent, int prmMin, int prmMax, int prmDefault);
        ~NumericSettingsPageItem() {}
        void Increment();
        void Decrement();
        string GetValueAsString();
        string GetTitle();
        PageItemType GetType();
        int GetValue();
        int GetMin();
        int GetMax();

    private:
        string title;
        int value;
        int min;
        int max;
    };
}

#endif