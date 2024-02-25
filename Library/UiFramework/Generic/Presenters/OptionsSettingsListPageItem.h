#pragma once
#ifndef OPTIONS_SETTINGS_PAGE_ITEM_H
#define OPTIONS_SETTINGS_PAGE_ITEM_H

#include "SettingsListPageItem.h"
#include "Option.h"
#include "ListPage.h"
#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class OptionsSettingsListPageItem : public SettingsListPageItem
    {
    public:
        OptionsSettingsListPageItem() {}
        OptionsSettingsListPageItem(string prmTitle, ListPage *prmParent);
        ~OptionsSettingsListPageItem() {}
        void Init(string prmTitle, ListPage *prmParent);
        void Increment();
        void Decrement();
        string GetValueAsString();
        void AddOption(string title, int value);
        string GetTitle();
        ListPageItemType GetType();
        uint8_t GetValue();

    private:
        string title;
        vector<Option> options;
        unsigned int currentIndex;
    };
}

#endif
