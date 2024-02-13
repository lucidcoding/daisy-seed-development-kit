#pragma once
#ifndef SETTINGS_PAGE_ITEM_H
#define SETTINGS_PAGE_ITEM_H

#include <string>
#include "ListPage.h"
#include "PageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::tree::presenters
{
    class SettingsPageItem : public PageItem
    {
    public:
        SettingsPageItem() {}
        ~SettingsPageItem() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        void Select();
        virtual string GetTitle() = 0;
        virtual string GetValueAsString() = 0;
        virtual PageItemType GetType() = 0;

    private:
    
    protected:
        ListPage *parent;
    };
}

#endif