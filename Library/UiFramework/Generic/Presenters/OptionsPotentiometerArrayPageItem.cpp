#include <cmath>
#include "OptionsPotentiometerArrayPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    const char *OptionsPotentiometerArrayPageItem::GetDisplayString()
    {
        return options[displayValue].title;
    }

    void OptionsPotentiometerArrayPageItem::AddOption(const char *title, int value)
    {
        Option option;
        option.title = title;
        option.value = value;
        options.push_back(option);
        SetScaledRange(0, options.size() - 1);
    }

    uint8_t OptionsPotentiometerArrayPageItem::GetSelectedValue()
    {
        return options[displayValue].value;
    }
}