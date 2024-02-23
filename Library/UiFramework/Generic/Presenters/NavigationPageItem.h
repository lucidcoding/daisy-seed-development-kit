#pragma once
#ifndef NAVIGATION_PAGE_ITEM_H
#define NAVIGATION_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"
#include "PageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class NavigationPageItem : public PageItem
    {
    public:
        NavigationPageItem() {}
        ~NavigationPageItem() {}
        void Init(string prmTitle, Page *prmNext, Root *prmRoot);
        void Increment();
        void Decrement();
        void Select();
        string GetTitle();
        PageItemType GetType();

    private:
        Page *next;
        Root *root;
        string title;
    };
}

#endif