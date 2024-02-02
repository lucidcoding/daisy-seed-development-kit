#include "NumericSettingsPageItem.h"
#include "PageItemType.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    NumericSettingsPageItem::NumericSettingsPageItem(string prmTitle, ListPage *prmParent)
    {
        title = prmTitle;
        parent = prmParent;
        value = 0;
    }

    void NumericSettingsPageItem::Increment()
    {
        value++;
        //*target = value;
    }

    void NumericSettingsPageItem::Decrement()
    {
        value--;
        //*target = value;
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
        return PageItemType::NumericSettingsPageItem;
    }
}