#pragma once
#ifndef SETTINGS_PAGE_ITEM_H
#define SETTINGS_PAGE_ITEM_H

#include <string>
#include "ListPage.h"
#include "ListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class SettingsListPageItem : public ListPageItem
    {
    public:
        SettingsListPageItem() {}
        ~SettingsListPageItem() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        void Select();
        virtual string GetTitle() = 0;
        virtual string GetValueAsString() = 0;
        virtual ListPageItemType GetType() = 0;

    private:
    protected:
        ListPage *parent;
    };
}

#endif