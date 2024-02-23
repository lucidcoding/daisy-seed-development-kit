#pragma once
#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include <string>
#include <vector>
#include "PageItem.h"
#include "Page.h"
#include "../Views/View.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework::tree::view;
    
    class ListPage : public Page
    {
    public:
        ListPage() {}
        ~ListPage() {}
        void Init(View *prmView);
        virtual void Increment();
        virtual void Decrement();
        virtual void Select();
        PageItem *GetItem(uint8_t index);
        void AddItem(PageItem *pageItem);
        uint8_t ItemsCount();
        uint8_t GetCurrentIndex();
        bool GetItemSelected();
        void ToggleItemSelected();
        void Paint();

    private:
        vector<PageItem *> items;
        uint8_t currentIndex;
        bool itemSelected;
        View *view;
    };
}

#endif