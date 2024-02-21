#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "./Presenters/Page.h"
#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::tree
{
    using namespace developmentKit::library::uiFramework::tree::presenters;

    class Display
    {
    public:
        Display() {}
        ~Display () {}
        void Increment();
        void Decrement();
        void Select();
        void SetPage(Page *newPage);
        Page *GetCurrentPage();
        void SetCurrentPage(Page *newCurrentPage);
        void SetHomePage(Page *newHomePage);
        virtual void Paint() = 0;;

    protected:
        Page *homePage;
        Page *currentPage;
    };
}

#endif