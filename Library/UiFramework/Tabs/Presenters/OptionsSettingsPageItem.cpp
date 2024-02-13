#include "OptionsSettingsPageItem.h"
#include "Option.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::tabs::presenters
{
    OptionsSettingsPageItem::OptionsSettingsPageItem(string prmTitle, ListPage *prmParent)
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
    }

    void OptionsSettingsPageItem::Decrement()
    {
        if (currentIndex > 0)
        {
            currentIndex--;
        }
    }

    string OptionsSettingsPageItem::GetValueAsString()
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

    PageItem::PageItemType OptionsSettingsPageItem::GetType()
    {
        return PageItemType::OPTIONS_SETTINGS_PAGE_ITEM;
    }

    unsigned int OptionsSettingsPageItem::GetValue()
    {
        return options[currentIndex].value;
    }
}