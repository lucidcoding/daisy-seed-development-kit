#pragma once
#ifndef TAB_PAGE_H
#define TAB_PAGE_H

#include <vector>
#include <stdint.h>
#include "Page.h"
#include "TabPageItem.h"
#include "../Views/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::tree::view;

    class TabPage : public Page
    {
    public:
        TabPage() {}
        ~TabPage() {}
        void Init(View *prmView);
        virtual void Increment();
        virtual void Decrement();
        virtual void Select();
        virtual void SetPotentiometerValues(float *values);
        TabPageItem *GetItem(uint8_t index);
        void AddItem(TabPageItem *item);
        uint8_t ItemsCount();
        uint8_t GetCurrentIndex();
        void SetFocus() {}
        void Paint();

    private:
        vector<TabPageItem *> items;
        uint8_t currentIndex;
        bool itemSelected;
        View *view;
    };
}

#endif