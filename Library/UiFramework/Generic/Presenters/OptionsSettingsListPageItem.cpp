#include <cstring>
#include "OptionsSettingsListPageItem.h"
#include "Option.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    OptionsSettingsListPageItem::OptionsSettingsListPageItem(const char* prmTitle, ListPage *prmParent)
    {
        title = prmTitle;
        currentIndex = 0;
        parent = prmParent;
    }

    void OptionsSettingsListPageItem::Init(const char *prmTitle, ListPage *prmParent)
    {
        title = prmTitle;
        currentIndex = 0;
        parent = prmParent;
    }

    void OptionsSettingsListPageItem::Increment()
    {
        if (currentIndex < options.size() - 1)
        {
            currentIndex++;
        }
    }

    void OptionsSettingsListPageItem::Decrement()
    {
        if (currentIndex > 0)
        {
            currentIndex--;
        }
    }

    const char* OptionsSettingsListPageItem::GetValueAsString()
    {
        return options[currentIndex].title;
    }

    void OptionsSettingsListPageItem::AddOption(const char *title, int value)
    {
        Option option;
        option.title = title;
        option.value = value;
        options.push_back(option);
    }

    const char *OptionsSettingsListPageItem::GetTitle()
    {
        return title;
    }

    ListPageItem::ListPageItemType OptionsSettingsListPageItem::GetType()
    {
        return ListPageItemType::OPTIONS_SETTINGS_PAGE_ITEM;
    }

    uint8_t OptionsSettingsListPageItem::GetValue()
    {
        return options[currentIndex].value;
    }
}