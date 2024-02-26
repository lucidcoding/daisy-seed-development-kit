#include <cmath>
#include "OptionsPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    string OptionsPotentiometerArrayPageItem::GetDisplayString()
    {
        return options[displayValue].title;
    }

    void OptionsPotentiometerArrayPageItem::AddOption(string title, int value)
    {
        Option option;
        option.title = title;
        option.value = value;
        options.push_back(option);
        SetScaledRange(0, options.size() - 1);
    }
}