#pragma once
#ifndef OPTIONS_POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define OPTIONS__POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <vector>
#include "Page.h"
#include "Option.h"
#include "../Views/View.h"
#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework::tree::view;

    class OptionsPotentiometerArrayPageItem : public PotentiometerArrayPageItem
    {
    public:
        OptionsPotentiometerArrayPageItem() {}
        ~OptionsPotentiometerArrayPageItem() {}
        virtual const char *GetDisplayString();
        void AddOption(const char *title, int value);
        uint8_t GetSelectedValue();

    protected:
        vector<Option> options;
    };
}

#endif