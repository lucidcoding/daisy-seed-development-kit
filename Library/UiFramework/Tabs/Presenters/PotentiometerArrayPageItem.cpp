#include <cmath>
#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    PotentiometerArrayPageItem::PotentiometerArrayPageItem()
    {
        SetScaledRange(0, 255);
    }

    void PotentiometerArrayPageItem::SetInitialKnobPosition(float prmInitialValue)
    {
        initialKnobPosition = prmInitialValue;
    }

    void PotentiometerArrayPageItem::SetCurrentKnobPosition(float prmCurrentKnobPosiiton)
    {
        currentKnobPosition = prmCurrentKnobPosiiton;

        if (!initialKnobPositionSet)
        {
            initialKnobPosition = prmCurrentKnobPosiiton;
            initialKnobPositionSet = true;
        }

        if (!knobPositionAndOutputValueSynced)
        {
            if (knobMode == DIRECT)
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

    void PotentiometerArrayPageItem::SetOutputValue(float prmOutputValue)
    {
        outputValue = prmOutputValue;
    }

    float PotentiometerArrayPageItem::GetOutputValue()
    {
        return outputValue;
    }

    int16_t PotentiometerArrayPageItem::GetDisplayValue()
    {
        return displayValue;
    }

    void PotentiometerArrayPageItem::SetFocus()
    {
        initialKnobPositionSet = false;
        firstUpdateSinceFocus = true;
    }

    bool PotentiometerArrayPageItem::GetKnobPositionAndOutputValueSynced()
    {
        return knobPositionAndOutputValueSynced;
    }

    void PotentiometerArrayPageItem::SetKnobMode(KnobMode prmKnobMode)
    {
        knobMode = prmKnobMode;
    }

    void PotentiometerArrayPageItem::SetScaledRange(int16_t prmMinScaledValue, int16_t prmMaxScaledValue)
    {
        scaler.Init(prmMinScaledValue, prmMaxScaledValue);
        division = scaler.GetDivision();
        hysteresisBand = division / 4.0f;
    }
}