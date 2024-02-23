#include "NumericSettingsPageItem.h"
#include <string>

using namespace std;
using namespace developmentKit::library::uiFramework::presenters;

namespace developmentKit::library::uiFramework::presenters
{
    void NumericSettingsPageItem::Init(string prmTitle, ListPage *prmParent, int16_t prmMin, int16_t prmMax, int16_t prmDefault)
    {
        title = prmTitle;
        parent = prmParent;
        value = prmDefault;
        min = prmMin;
        max = prmMax;
    }

    void NumericSettingsPageItem::Increment()
    {
        if (value < max)
        {
            value++;
        }
    }

    void NumericSettingsPageItem::Decrement()
    {
        if (value > min)
        {
            value--;
        }
    }

    string NumericSettingsPageItem::GetValueAsString()
    {
        char buffer[6];
        sprintf(buffer, "%d", value);
        return buffer;
    }

    string NumericSettingsPageItem::GetTitle()
    {
        return title;
    }

    PageItem::PageItemType NumericSettingsPageItem::GetType()
    {
        return PageItemType::NUMERIC_SETTINGS_PAGE_ITEM;
    }

    int16_t NumericSettingsPageItem::GetValue()
    {
        return value;
    }

    int16_t NumericSettingsPageItem::GetMin()
    {
        return min;
    }

    int16_t NumericSettingsPageItem::GetMax()
    {
        return max;
    }

    float NumericSettingsPageItem::GetScaledValue()
    {
        float floatValue = (float)value;
        float floatMin = (float)min;
        float floatMax = (float)max;
        return (floatValue / (floatMax - floatMin)) - (floatMin / (floatMax - floatMin));
    }
}