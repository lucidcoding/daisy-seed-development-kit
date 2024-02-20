#pragma once
#ifndef CONTINUOUS_POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define CONTINUOUS_POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <string>
#include "Page.h"
#include "../View/View.h"
#include "PotentiometerArrayPageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;
    using namespace developmentKit::library::uiFramework::shared::scaling;

    class NumericPotentiometerArrayPageItem : public PotentiometerArrayPageItem
    {
    public:
        NumericPotentiometerArrayPageItem();
        ~NumericPotentiometerArrayPageItem() {}
        virtual string GetDisplayString();

    protected:
        int16_t Scale(float prmValue);
    };
}

#endif