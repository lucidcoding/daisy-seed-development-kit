#pragma once
#ifndef PAGE_ITEM_H
#define PAGE_ITEM_H

#include "PageItemType.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class PageItem
    {
    public:
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        virtual void Select() = 0;
        virtual string GetTitle() = 0;
        virtual PageItemType GetType() = 0;

    private:
    };
}

#endif