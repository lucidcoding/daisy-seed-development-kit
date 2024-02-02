#include "OptionsSettingsPageItem.h"
#include "Option.h"
#include "PageItemType.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    OptionsSettingsPageItem::OptionsSettingsPageItem(string prmTitle, /*unsigned int *prmTarget,*/ ListPage *prmParent)
    {
        title = prmTitle;
        currentIndex = 0;
        parent = prmParent;
    }

    void OptionsSettingsPageItem::Increment()
    {
        if (currentIndex < options.size() - 1)
        {
            currentIndex++;
        }

        *target = options[currentIndex].value;
    }

    void OptionsSettingsPageItem::Decrement()
    {
        if (currentIndex > 0)
        {
            currentIndex--;
        }

        *target = options[currentIndex].value;
    }

    string OptionsSettingsPageItem::GetValue()
    {
        return options[currentIndex].title;
    }

    void OptionsSettingsPageItem::AddOption(string title, int value)
    {
        Option option;
        option.title = title;
        option.value = value;
        options.push_back(option);
    }

    string OptionsSettingsPageItem::GetTitle()
    {
        return title;
    }

    PageItemType OptionsSettingsPageItem::GetType()
    {
        return PageItemType::OptionsSettingsPageItem;
    }
}