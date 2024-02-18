#include <cmath>
#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
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
        return outputValue * 256;
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
}