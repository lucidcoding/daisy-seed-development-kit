#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_ITEM_H
#define POTENTIOMETER_ARRAY_PAGE_ITEM_H

#include "Page.h"
#include "../Views/View.h"
#include "../Utilities/PotentiometerToDisplayValueScaler.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework::views;
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
        void SetTitle(const char *prmTitle);
        const char *GetTitle();
        void InitialiseKnobPosition(float prmInitialValue);
        void SetOutputValue(float prmOutputValue);
        void SetCurrentKnobPosition(float prmCurrentKnobPosiiton);
        float GetOutputValue();
        int16_t GetDisplayValue();
        virtual const char *GetDisplayString() = 0;
        void SetFocus();
        bool GetKnobPositionAndOutputValueSynced();
        void SetKnobMode(KnobMode prmKnobMode);
        void SetScaledRange(int16_t prmMinScaledValue, int16_t prmMaxScaledValue);

    protected:
        const char *title;
        bool initialKnobPositionSet;
        float initialKnobPosition;
        float currentKnobPosition;
        float outputValue;
        int16_t displayValue;
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