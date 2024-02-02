#include "NumericSettingsPageItem.h"
#include "PageItemType.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    NumericSettingsPageItem::NumericSettingsPageItem(string prmTitle, ListPage *prmParent, int prmDisplayMin, int prmDisplayMax, float prmOutputMin, float prmOutputMax, Curve prmCurve, float* prmTarget)
    {
        title = prmTitle;
        parent = prmParent;
        value = 0;
        target = prmTarget;
    }

    void NumericSettingsPageItem::Increment()
    {
        value++;
        //*target = value;
        *target = (float)value / 255.0f;
    }

    void NumericSettingsPageItem::Decrement()
    {
        value--;
        //*target = value;
        *target = (float)value / 255.0f;
    }

    string NumericSettingsPageItem::GetValue()
    {
        char buffer[6];
        sprintf(buffer, "%d", value);
        return buffer;
    }

    string NumericSettingsPageItem::GetTitle()
    {
        return title;
    }

    PageItemType NumericSettingsPageItem::GetType()
    {
        return PageItemType::NUMERIC_SETTINGS_PAGE_ITEM;
    }
}