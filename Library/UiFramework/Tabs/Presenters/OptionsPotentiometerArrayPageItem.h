#pragma once
#ifndef OPTIONS_POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define OPTIONS__POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <string>
#include <vector>
#include "Page.h"
#include "Option.h"
#include "../View/View.h"
#include "PotentiometerArrayPageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;

    class OptionsPotentiometerArrayPageItem : public PotentiometerArrayPageItem
    {
    public:
        OptionsPotentiometerArrayPageItem() {}
        ~OptionsPotentiometerArrayPageItem() {}
        virtual string GetDisplayString();
        void AddOption(string title, int value);

    protected:
        vector<Option> options;
    };
}

#endif