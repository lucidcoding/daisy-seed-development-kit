#pragma once
#ifndef OPTIONS_POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define OPTIONS__POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <string>
#include "Page.h"
#include "../View/View.h"
#include "../../Shared/Scaling/PotentiometerToDisplayValueScaler.h"
#include "PotentiometerArrayPageItem.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;
    using namespace developmentKit::library::uiFramework::shared::scaling;

    class OptionsPotentiometerArrayPageItem : public PotentiometerArrayPageItem
    {
    public:
        OptionsPotentiometerArrayPageItem();
        ~OptionsPotentiometerArrayPageItem() {}
        void SetCurrentKnobPosition(float prmCurrentKnobPosiiton);
        int16_t GetDisplayValue();
        string GetDisplayString();
        void SetFocus();
        void SetScaledRange(int16_t prmMinScaledValue, int16_t prmMaxScaledValue);
        void SetOptions(string *prmOptions);

    private:
        string *options;
        //PotentiometerToDisplayValueScaler scaler;
        /*bool initialKnobPositionSet;
        float initialKnobPosition;
        float currentKnobPosition;
        float outputValue;
        int16_t displayValue;
        int16_t minScaledValue;
        int16_t maxScaledValue;
        KnobMode knobMode = DIRECT;
        bool knobPositionAndOutputValueSynced = false;
        const float amountToRegisterForSync = 0.004f;
        bool firstUpdateSinceFocus = true;
        float division;
        float hysteresisBand;
        string *options;*/
    };
}

#endif