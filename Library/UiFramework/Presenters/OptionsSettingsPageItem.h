#pragma once
#ifndef OPTIONS_SETTINGS_PAGE_ITEM_H
#define OPTIONS_SETTINGS_PAGE_ITEM_H

//#include "ListPage.h"
#include "SettingsPageItem.h"
#include "Option.h"
#include "PageItemType.h"
#include <string>
#include <vector>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class OptionsSettingsPageItem : public SettingsPageItem
    {
    public:
        OptionsSettingsPageItem(string prmTitle, /*unsigned int *prmTarget,*/ ListPage *prmParent);
        void Increment();
        void Decrement();
        string GetValue();
        void AddOption(string title, int value);
        string GetTitle();
        PageItemType GetType();

    private:
        string title;
        unsigned int *target;
        vector<Option> options;
        unsigned int currentIndex;
    };
}

#endif
