#pragma once
#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::presenters
{
    class Page
    {
    public:
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        virtual void Select() = 0;
        virtual void Paint() = 0;

    private:
    };
}

#endif