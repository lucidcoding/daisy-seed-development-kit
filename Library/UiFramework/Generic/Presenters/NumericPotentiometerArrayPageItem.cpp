#include <cmath>
#include "NumericPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    NumericPotentiometerArrayPageItem::NumericPotentiometerArrayPageItem()
    {
        SetScaledRange(0, 255);
    }
    
    string NumericPotentiometerArrayPageItem::GetDisplayString()
    {
        char buffer[8];
        sprintf(buffer, "%d", displayValue);
        return buffer;
    }
}