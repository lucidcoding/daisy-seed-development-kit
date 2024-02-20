#include <cmath>
#include "NumericPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    NumericPotentiometerArrayPageItem::NumericPotentiometerArrayPageItem()
    {
        SetScaledRange(0, 255);
    }
    
    string NumericPotentiometerArrayPageItem::GetDisplayString()
    {
        char buffer[6];
        sprintf(buffer, "%d", displayValue);
        return buffer;
    }
}