#pragma once
#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include <vector>
#include "ListPageItem.h"
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
        virtual void SetPotentiometerValues(float *values) {}
        virtual void SetFocus() {}
        ListPageItem *GetItem(uint8_t index);
        void AddItem(ListPageItem *pageItem);
        uint8_t ItemsCount();
        uint8_t GetCurrentIndex();
        bool GetItemSelected();
        void ToggleItemSelected();
        void Paint();

    private:
        vector<ListPageItem *> items;
        uint8_t currentIndex;
        bool itemSelected;
        View *view;
    };
}

#endif