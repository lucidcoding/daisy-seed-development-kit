#pragma once
#ifndef OPTIONS_SETTINGS_PAGE_ITEM_H
#define OPTIONS_SETTINGS_PAGE_ITEM_H

#include <vector>
#include <stdint.h>
#include "SettingsListPageItem.h"
#include "Option.h"
#include "ListPage.h"

namespace developmentKit::library::uiFramework::presenters
{
    class OptionsSettingsListPageItem : public SettingsListPageItem
    {
    public:
        OptionsSettingsListPageItem() {}
        OptionsSettingsListPageItem(const char *prmTitle, ListPage *prmParent);
        ~OptionsSettingsListPageItem() {}
        void Init(const char *prmTitle, ListPage *prmParent);
        void Increment();
        void Decrement();
        const char *GetValueAsString();
        void AddOption(const char *title, int value);
        const char *GetTitle();
        ListPageItemType GetType();
        uint8_t GetValue();

    private:
        const char *title;
        vector<Option> options;
        unsigned int currentIndex;
    };
}

#endif
