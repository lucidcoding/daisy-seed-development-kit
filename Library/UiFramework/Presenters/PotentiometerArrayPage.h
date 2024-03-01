#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_H
#define POTENTIOMETER_ARRAY_PAGE_H

#include <vector>
#include "Page.h"
#include "PotentiometerArrayPageItem.h"
#include "../Views/View.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework::views;

    class PotentiometerArrayPage : public Page
    {
    public:
        PotentiometerArrayPage() {}
        ~PotentiometerArrayPage() {}
        void Init(View *prmView);
        virtual void Increment();
        virtual void Decrement();
        virtual void SetPotentiometerValues(float *values);
        void Select();
        void SetFocus();
        void SetOutputValues(float *values);
        void Paint();
        PotentiometerArrayPageItem *GetItem(uint8_t index);
        void SetItem(uint8_t index, PotentiometerArrayPageItem *item);
        void SetKnobMode(PotentiometerArrayPageItem::KnobMode prmKnobMode);

    private:
        View *view;
        PotentiometerArrayPageItem *items[16];
    };
}

#endif