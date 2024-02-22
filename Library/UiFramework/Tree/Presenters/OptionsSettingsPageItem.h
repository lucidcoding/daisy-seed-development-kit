#pragma once
#ifndef OPTIONS_SETTINGS_PAGE_ITEM_H
#define OPTIONS_SETTINGS_PAGE_ITEM_H

// #include "ListPage.h"
#include "SettingsPageItem.h"
#include "Option.h"
#include <string>
#include <vector>

using namespace std;

namespace developmentKit::library::uiFramework::tree::presenters
{
    class OptionsSettingsPageItem : public SettingsPageItem
    {
    public:
        OptionsSettingsPageItem() {}
        OptionsSettingsPageItem(string prmTitle, ListPage *prmParent);
        ~OptionsSettingsPageItem() {}
        void Init(string prmTitle, ListPage *prmParent);
        void Increment();
        void Decrement();
        string GetValueAsString();
        void AddOption(string title, int value);
        string GetTitle();
        PageItemType GetType();
        unsigned int GetValue();

    private:
        string title;
        vector<Option> options;
        unsigned int currentIndex;
    };
}

#endif
