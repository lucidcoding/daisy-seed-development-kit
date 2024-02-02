#pragma once
#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include <string>
#include <vector>
#include "PageItem.h"
#include "Page.h"

using namespace std;

namespace developmentKit::library::uiFramework::view
{
    class ListPageView;
}

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework::view;
    
    class ListPage : public Page
    {
    public:
        ListPage(ListPageView *prmView);
        virtual void Increment();
        virtual void Decrement();
        virtual void Select();
        PageItem *GetItem(unsigned int index);
        void AddItem(PageItem *pageItem);
        unsigned int ItemsCount();
        unsigned int GetCurrentIndex();
        bool GetItemSelected();
        void ToggleItemSelected();
        void Paint();

    private:
        vector<PageItem *> items;
        unsigned int currentIndex;
        bool itemSelected;
        ListPageView *view;
    };
}

#endif