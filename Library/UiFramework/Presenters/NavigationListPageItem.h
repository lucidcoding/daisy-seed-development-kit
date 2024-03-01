#pragma once
#ifndef NAVIGATION_PAGE_ITEM_H
#define NAVIGATION_PAGE_ITEM_H

#include "Page.h"
#include "ListPageItem.h"
#include "Root.h"

namespace developmentKit::library::uiFramework::presenters
{
    class NavigationListPageItem : public ListPageItem
    {
    public:
        NavigationListPageItem() {}
        ~NavigationListPageItem() {}
        void Init(const char *prmTitle, Page *prmNext, Root *prmRoot);
        void Increment();
        void Decrement();
        void Select();
        const char *GetTitle();
        ListPageItemType GetType();

    private:
        Page *next;
        Root *root;
        const char *title;
    };
}

#endif