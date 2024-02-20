#include <cmath>
#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    void PotentiometerArrayPageItem::SetInitialKnobPosition(float prmInitialValue)
    {
        initialKnobPosition = prmInitialValue;
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
}