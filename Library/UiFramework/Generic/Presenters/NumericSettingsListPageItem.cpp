#include <stdio.h>
#include "NumericSettingsListPageItem.h"

using namespace developmentKit::library::uiFramework::presenters;

namespace developmentKit::library::uiFramework::presenters
{
    void NumericSettingsListPageItem::Init(const char *prmTitle, ListPage *prmParent, int16_t prmMin, int16_t prmMax, int16_t prmDefault)
    {
        title = prmTitle;
        parent = prmParent;
        value = prmDefault;
        min = prmMin;
        max = prmMax;
    }

    void NumericSettingsListPageItem::Increment()
    {
        if (value < max)
        {
            value++;
        }
    }

    void NumericSettingsListPageItem::Decrement()
    {
        if (value > min)
        {
            value--;
        }
    }

    const char *NumericSettingsListPageItem::GetValueAsString()
    {
        sprintf(displayString, "%d", value);
        return displayString;
    }

    const char *NumericSettingsListPageItem::GetTitle()
    {
        return title;
    }

    ListPageItem::ListPageItemType NumericSettingsListPageItem::GetType()
    {
        return ListPageItemType::NUMERIC_SETTINGS_PAGE_ITEM;
    }

    int16_t NumericSettingsListPageItem::GetValue()
    {
        return value;
    }

    int16_t NumericSettingsListPageItem::GetMin()
    {
        return min;
    }

    int16_t NumericSettingsListPageItem::GetMax()
    {
        return max;
    }

    float NumericSettingsListPageItem::GetScaledValue()
    {
        float floatValue = (float)value;
        float floatMin = (float)min;
        float floatMax = (float)max;
        return (floatValue / (floatMax - floatMin)) - (floatMin / (floatMax - floatMin));
    }
}