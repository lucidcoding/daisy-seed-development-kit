#pragma once
#ifndef PAGE_H
#define PAGE_H

namespace developmentKit::library::uiFramework::presenters
{
    class Page
    {
    public:
        Page() {}
        ~Page() {}
        virtual void Increment() = 0;
        virtual void Decrement() = 0;
        virtual void Select() = 0;
        virtual void SetPotentiometerValues(float *values) = 0;
        virtual void Up() = 0;
        virtual void Left() = 0;
        virtual void Right() = 0;
        virtual void Down() = 0;
        virtual void Back() = 0;
        virtual void Menu() = 0;
        virtual void SetFocus() = 0;
        virtual void Paint() = 0;

    private:
    };
}

#endif