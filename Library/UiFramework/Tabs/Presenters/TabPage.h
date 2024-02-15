#pragma once
#ifndef TAB_PAGE_H
#define TAB_PAGE_H

#include <string>
#include <vector>
#include "Page.h"
#include "TabPageItem.h"
#include "../View/View.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;

    class TabPage : public Page
    {
    public:
        TabPage(View *prmView);
        ~TabPage() {}
        virtual void Increment();
        virtual void Decrement();
        virtual void Select();
        virtual void SetPotentiometerValue(uint8_t index, float value);
        virtual void SetPotentiometerValues(float *values);
        TabPageItem *GetItem(uint8_t index);
        void AddItem(TabPageItem *item);
        uint8_t ItemsCount();
        uint8_t GetCurrentIndex();
        void Paint();

    private:
        vector<TabPageItem *> items;
        uint8_t currentIndex;
        bool itemSelected;
        View *view;
    };
}

#endif