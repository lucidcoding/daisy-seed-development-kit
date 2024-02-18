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
        enum KnobMode
        {
            DIRECT,
            CATCH
        };
        PotentiometerArrayPageItem() {}
        ~PotentiometerArrayPageItem() {}
        void SetInitialKnobPosition(float prmInitialValue);
        void SetOutputValue(float prmOutputValue);
        void SetCurrentKnobPosition(float prmCurrentKnobPosiiton);
        float GetOutputValue();
        int16_t GetDisplayValue();
        void SetFocus();
        bool GetKnobPositionAndOutputValueSynced();

        // Testing only
        float GetInitialKnobPosition() { return initialKnobPosition; }

    private:
        bool initialKnobPositionSet;
        float initialKnobPosition;
        float currentKnobPosition;
        float outputValue;
        int16_t displayValue;
        KnobMode knobMode = DIRECT;
        bool knobPositionAndOutputValueSynced = false;
        const float amountToRegisterForSync = 0.004f;
        bool firstUpdateSinceFocus = true;
    };
}

#endif