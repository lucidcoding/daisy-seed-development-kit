#include "NumericSettingsPageItem.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::tree::presenters
{
    NumericSettingsPageItem::NumericSettingsPageItem(string prmTitle, ListPage *prmParent, int prmMin, int prmMax, int prmDefault)
    {
        title = prmTitle;
        parent = prmParent;
        value = prmDefault;
        min = prmMin;
        max = prmMax;
    }

    void NumericSettingsPageItem::Init(string prmTitle, ListPage *prmParent, int prmMin, int prmMax, int prmDefault)
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

    int NumericSettingsPageItem::GetValue()
    {
        return value;
    }

    int NumericSettingsPageItem::GetMin()
    {
        return min;
    }

    int NumericSettingsPageItem::GetMax()
    {
        return max;
    }
}