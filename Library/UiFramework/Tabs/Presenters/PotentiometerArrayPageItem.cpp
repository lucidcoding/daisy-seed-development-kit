#include "PotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    void PotentiometerArrayPageItem::SetInitialValue(float prmInitialValue)
    {
        initialValue = prmInitialValue;
    }

    void PotentiometerArrayPageItem::SetRawValue(float prmRawValue)
    {
        rawValue = prmRawValue;
    }

    int16_t PotentiometerArrayPageItem::GetDisplayValue()
    {
        return rawValue * 256;
    }
}