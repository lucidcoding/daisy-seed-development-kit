#pragma once
#ifndef NAVIGATION_PAGE_ITEM_H
#define NAVIGATION_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"
#include "PageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework
{
    class Display;
}

namespace developmentKit::library::uiFramework::presenters
{
    class NavigationPageItem : public PageItem
    {
    public:
        NavigationPageItem(string prmTitle, Page *prmNext, Display *prmRoot);
        ~NavigationPageItem() {}
        void Increment();
        void Decrement();
        void Select();
        string GetTitle();
        PageItemType GetType();

    private:
        Page *next;
        Display *root;
        string title;
    };
}

#endif