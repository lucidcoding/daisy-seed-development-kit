#pragma once
#ifndef NAVIGATION_PAGE_ITEM_H
#define NAVIGATION_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"
#include "ListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class NavigationListPageItem : public ListPageItem
    {
    public:
        NavigationListPageItem() {}
        ~NavigationListPageItem() {}
        void Init(string prmTitle, Page *prmNext, Root *prmRoot);
        void Increment();
        void Decrement();
        void Select();
        string GetTitle();
        ListPageItemType GetType();

    private:
        Page *next;
        Root *root;
        string title;
    };
}

#endif