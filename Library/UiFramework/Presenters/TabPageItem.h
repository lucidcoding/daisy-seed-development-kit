#pragma once
#ifndef TAB_PAGE_ITEM_H
#define TAB_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class TabPageItem
    {
    public:
        TabPageItem(string prmTitle, Page *prmContent);
        ~TabPageItem() {}
        /*virtual void Increment();
        virtual void Decrement();
        virtual void Select();*/
        Page *GetContent();
        string GetTitle();

    private:
        Page *content;
        string title;
    };
}

#endif