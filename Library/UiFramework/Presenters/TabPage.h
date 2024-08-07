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
    using namespace developmentKit::library::uiFramework::views;

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
        virtual void Up() {}
        virtual void Left();
        virtual void Right();
        virtual void Down() {}
        virtual void Back() {}
        virtual void Menu();
        TabPageItem *GetItem(uint8_t index);
        void AddItem(TabPageItem *item);
        uint8_t ItemsCount();
        uint8_t GetCurrentIndex();
        bool GetMenuVisible();
        void SetMenuContent(Page *newMenuContent);
        Page *GetMenuContent();
        void SetFocus() {}
        void Paint();

    private:
        vector<TabPageItem *> items;
        Page *menuContent;
        uint8_t currentIndex;
        bool itemSelected;
        View *view;
        bool menuVisible;
    };
}

#endif