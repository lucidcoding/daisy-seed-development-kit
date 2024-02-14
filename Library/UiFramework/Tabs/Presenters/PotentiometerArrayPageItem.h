#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"
#include "../View/View.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;
    
    class PotentiometerArrayPageItem 
    {
    public:
        PotentiometerArrayPageItem() {}
        ~PotentiometerArrayPageItem() {}
        void SetInitialValue(float prmInitialValue);
        void SetRawValue(float prmRawValue);
        int16_t GetDisplayValue();
        //void Paint();

    private:
        //View *view;
        float initialValue;
        float rawValue;
        int16_t displayValue;
    };
}

#endif