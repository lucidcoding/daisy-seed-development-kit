#pragma once
#ifndef NUMERIC_SETTINGS_PAGE_ITEM_H
#define NUMERIC_SETTINGS_PAGE_ITEM_H

#include <string>
#include <vector>
#include "SettingsPageItem.h"
#include "PageItemType.h"

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    class NumericSettingsPageItem : public SettingsPageItem
    {
    public:
        enum Curve
        {
            LINEAR, 
            EXPONENTIAL, 
            LOGARITHMIC, 
            CUBE,    
            LAST,  
        };
        NumericSettingsPageItem(string prmTitle, ListPage *prmParent, int prmDisplayMin, int prmDisplayMax, float prmOutputMin, float prmOutputMax, Curve prmCurve, float *prmTarget);
        void Increment();
        void Decrement();
        string GetValue();
        string GetTitle();
        PageItemType GetType();

    private:
        float *target;
        unsigned int value;
        string title;
        int displayMin;
        int displayMax;
        float outputMin;
        float outputMax;
        Curve curve;
    };
}

#endif