#pragma once
#ifndef SETTINGS_PAGE_ITEM_H
#define SETTINGS_PAGE_ITEM_H

#include <string>
#include "ListPage.h"
#include "PageItem.h"
#include "PageItemType.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class SettingsPageItem : public PageItem
    {
    public:
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        void Select();
        virtual string GetTitle() = 0;
        virtual string GetValue() = 0;
        virtual PageItemType GetType() = 0;

    private:
    protected:
        ListPage *parent;
    };
}

#endif