#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include <string>
#include "Page.h"
#include "../View/View.h"
#include "../../Shared/Scaling/PotentiometerToDisplayValueScaler.h"

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    using namespace developmentKit::library::uiFramework::tabs::view;
    using namespace developmentKit::library::uiFramework::shared::scaling;

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
        virtual void SetCurrentKnobPosition(float prmCurrentKnobPosiiton) = 0;
        float GetOutputValue();
        int16_t GetDisplayValue();
        virtual string GetDisplayString() = 0;
        void SetFocus();
        bool GetKnobPositionAndOutputValueSynced();
        void SetKnobMode(KnobMode prmKnobMode);

    protected:
        bool initialKnobPositionSet;
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
        PotentiometerToDisplayValueScaler scaler;
    };
}

#endif