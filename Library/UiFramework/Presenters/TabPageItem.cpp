#include "TabPageItem.h"
#include "Page.h"
#include "../View/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    TabPageItem::TabPageItem(string prmTitle, Page *prmContent)
    {
        title = prmTitle;
        content = prmContent;
    }

    Page *TabPageItem::GetContent()
    {
        return content;
    }

    string TabPageItem::GetTitle()
    {
        return title;
    }
}