#pragma once
#ifndef TREE_ROOT_H
#define TREE_ROOT_H

#include "./Page.h"
#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::tree::presenters
{
    class TreeRoot
    {
    public:
        TreeRoot() {}
        ~TreeRoot () {}
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