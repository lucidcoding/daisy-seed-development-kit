#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "./Presenters/Page.h"
#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::tabs
{
    using namespace developmentKit::library::uiFramework::tabs::presenters;

    class Display
    {
    public:
        Display() {}
        ~Display () {}
        void Increment();
        void Decrement();
        void Select();
        void SetPotentiometerValue(uint8_t index, float value);
        void SetPage(Page *newPage);
        Page *GetCurrentPage();
        void SetCurrentPage(Page *newCurrentPage);
        void SetHomePage(Page *newHomePage);
        void Paint();

    private:
        Page *homePage;
        Page *currentPage;
    };
}

#endif