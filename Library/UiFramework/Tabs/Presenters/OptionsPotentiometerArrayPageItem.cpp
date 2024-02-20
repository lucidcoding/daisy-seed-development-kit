#include <cmath>
#include "OptionsPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    OptionsPotentiometerArrayPageItem::OptionsPotentiometerArrayPageItem()
    {
        SetScaledRange(0, 255);
        options = NULL;
    }

    void OptionsPotentiometerArrayPageItem::SetCurrentKnobPosition(float prmCurrentKnobPosiiton)
    {
        currentKnobPosition = prmCurrentKnobPosiiton;

        if (!initialKnobPositionSet)
        {
            initialKnobPosition = prmCurrentKnobPosiiton;
            initialKnobPositionSet = true;
        }

        if (!knobPositionAndOutputValueSynced)
        {
            if (knobMode == PotentiometerArrayPageItem::DIRECT)
            {
                if (abs(currentKnobPosition - initialKnobPosition) > amountToRegisterForSync)
                {
                    knobPositionAndOutputValueSynced = true;
                }
            }
            else
            {
                if ((initialKnobPosition > outputValue && currentKnobPosition < outputValue) || (initialKnobPosition < outputValue && currentKnobPosition > outputValue))
                {
                    knobPositionAndOutputValueSynced = true;
                }
            }
        }

        if (firstUpdateSinceFocus)
        {
            if (abs(currentKnobPosition - outputValue) > amountToRegisterForSync)
            {
                knobPositionAndOutputValueSynced = false;
            }
            else
            {
                knobPositionAndOutputValueSynced = true;
            }
        }

        if (knobPositionAndOutputValueSynced)
        {
            outputValue = currentKnobPosition;
        }

        // if new value is only one more and raw value is towrds the lower end of the new value
        int16_t newDisplayValue = scaler.Scale(outputValue);

        if ((newDisplayValue == displayValue + 1) && outputValue > (division * (float)(displayValue + 1)) + hysteresisBand)
        {
            displayValue = newDisplayValue;
        }
        else if (newDisplayValue > displayValue + 1)
        {
            displayValue = newDisplayValue;
        }
        else if ((newDisplayValue == displayValue - 1) && outputValue < (float)(division * displayValue) - hysteresisBand)
        {
            displayValue = newDisplayValue;
        }
        else if (newDisplayValue < displayValue - 1)
        {
            displayValue = newDisplayValue;
        }
        // displayValue = newDisplayValue;

        firstUpdateSinceFocus = false;
    }

    int16_t OptionsPotentiometerArrayPageItem::GetDisplayValue()
    {
        return displayValue;
    }

    string OptionsPotentiometerArrayPageItem::GetDisplayString()
    {
        return options[displayValue];
    }

    void OptionsPotentiometerArrayPageItem::SetFocus()
    {
        initialKnobPositionSet = false;
        firstUpdateSinceFocus = true;
    }

    void OptionsPotentiometerArrayPageItem::SetScaledRange(int16_t prmMinScaledValue, int16_t prmMaxScaledValue)
    {
        scaler.Init(prmMinScaledValue, prmMaxScaledValue);
        division = scaler.GetDivision();
        hysteresisBand = division / 4.0f;
    }

    void OptionsPotentiometerArrayPageItem::SetOptions(string *prmOptions)
    {
        options = prmOptions;
    }
}