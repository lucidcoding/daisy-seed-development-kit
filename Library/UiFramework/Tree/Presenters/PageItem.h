#pragma once
#ifndef PAGE_ITEM_H
#define PAGE_ITEM_H

#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::tree::presenters
{
    class PageItem
    {
    public:
        enum PageItemType
        {
            NAVIGATION_PAGE_ITEM,
            NUMERIC_SETTINGS_PAGE_ITEM,
            OPTIONS_SETTINGS_PAGE_ITEM
        };
        PageItem() {}
        ~PageItem() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        virtual void Select() = 0;
        virtual string GetTitle() = 0;
        virtual PageItemType GetType() = 0;

    private:
    };
}

#endif