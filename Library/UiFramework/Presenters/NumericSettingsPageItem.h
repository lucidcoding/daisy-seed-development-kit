#pragma once
#ifndef NUMERIC_SETTINGS_PAGE_ITEM_H
#define NUMERIC_SETTINGS_PAGE_ITEM_H

#include <string>
#include <vector>
#include "SettingsPageItem.h"
#include "PageItemType.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class NumericSettingsPageItem : public SettingsPageItem
    {
    public:
        NumericSettingsPageItem(string prmTitle, ListPage *prmParent);
        void Increment();
        void Decrement();
        string GetValue();
        string GetTitle();
        PageItemType GetType();

    private:
        unsigned int *target;
        unsigned int value;
        string title;
    };
}

#endif