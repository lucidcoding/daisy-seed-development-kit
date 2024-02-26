#pragma once
#ifndef TREE_ROOT_H
#define TREE_ROOT_H

#include "./Page.h"
#include <vector>

namespace developmentKit::library::uiFramework::presenters
{
    class Root
    {
    public:
        Root() {}
        ~Root () {}
        void Increment();
        void Decrement();
        void Select();
        void SetPotentiometerValues(float *values);
        void SetPage(Page *newPage);
        Page *GetCurrentPage();
        void SetCurrentPage(Page *newCurrentPage);
        void SetHomePage(Page *newHomePage);
        void Paint();

    protected:
        Page *homePage;
        Page *currentPage;
    };
}

#endif