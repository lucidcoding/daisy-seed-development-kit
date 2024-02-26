#pragma once
#ifndef SETTINGS_PAGE_ITEM_H
#define SETTINGS_PAGE_ITEM_H

#include "ListPage.h"
#include "ListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    class SettingsListPageItem : public ListPageItem
    {
    public:
        SettingsListPageItem() {}
        ~SettingsListPageItem() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        void Select();
        virtual const char *GetTitle() = 0;
        virtual const char *GetValueAsString() = 0;
        virtual ListPageItemType GetType() = 0;

    private:
    protected:
        ListPage *parent;
    };
}

#endif