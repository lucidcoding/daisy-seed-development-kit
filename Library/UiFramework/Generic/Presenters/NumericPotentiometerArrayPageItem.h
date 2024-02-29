#pragma once
#ifndef CONTINUOUS_POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define CONTINUOUS_POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <stdint.h>
#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{

    class NumericPotentiometerArrayPageItem : public PotentiometerArrayPageItem
    {
    public:
        NumericPotentiometerArrayPageItem();
        ~NumericPotentiometerArrayPageItem() {}
        virtual const char *GetDisplayString();

    protected:
        int16_t Scale(float prmValue);
        char displayString[16];
    };
}

#endif