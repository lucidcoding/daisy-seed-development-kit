#include <stdio.h>
#include "NumericPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    NumericPotentiometerArrayPageItem::NumericPotentiometerArrayPageItem()
    {
        SetScaledRange(0, 255);
    }

    const char *NumericPotentiometerArrayPageItem::GetDisplayString()
    { 
        sprintf(displayString, "%d", displayValue);
        return displayString;
    }
}