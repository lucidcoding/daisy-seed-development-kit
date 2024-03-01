#pragma once
#ifndef TAB_PAGE_ITEM_H
#define TAB_PAGE_ITEM_H

#include <vector>
#include "Page.h"

namespace developmentKit::library::uiFramework::presenters
{
    class TabPageItem
    {
    public:
        TabPageItem() {}
        TabPageItem(const char *prmTitle, Page *prmContent);
        ~TabPageItem() {}
        void Init(const char *prmTitle, Page *prmContent);
        Page *GetContent();
        const char *GetTitle();

    private:
        Page *content;
        const char *title;
    };
}

#endif