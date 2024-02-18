#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_H
#define POTENTIOMETER_ARRAY_PAGE_H

#include <string>
#include <vector>
#include "Page.h"
#include "../View/View.h"
#include "PotentiometerArrayPageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;
    
    class PotentiometerArrayPage : public Page
    {
    public:
        PotentiometerArrayPage(View *prmView);
        ~PotentiometerArrayPage() {}
        virtual void Increment();
        virtual void Decrement();
        virtual void SetPotentiometerValue(uint8_t index, float value);
        virtual void SetPotentiometerValues(float *values);
        void Select();
        void SetFocus();
        void SetOutputValues(float *values);
        void Paint();
        PotentiometerArrayPageItem * GetItem(uint8_t index);
        void SetKnobMode(PotentiometerArrayPageItem::KnobMode prmKnobMode);

    private:
        View *view;
        PotentiometerArrayPageItem items[16];
    };
}

#endif