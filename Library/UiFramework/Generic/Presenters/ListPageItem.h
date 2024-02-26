#pragma once
#ifndef PAGE_ITEM_H
#define PAGE_ITEM_H

namespace developmentKit::library::uiFramework::presenters
{
    class ListPageItem
    {
    public:
        enum ListPageItemType
        {
            NAVIGATION_PAGE_ITEM,
            NUMERIC_SETTINGS_PAGE_ITEM,
            OPTIONS_SETTINGS_PAGE_ITEM
        };
        ListPageItem() {}
        ~ListPageItem() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        virtual void Select() = 0;
        virtual const char *GetTitle() = 0;
        virtual ListPageItemType GetType() = 0;

    private:
    };
}

#endif