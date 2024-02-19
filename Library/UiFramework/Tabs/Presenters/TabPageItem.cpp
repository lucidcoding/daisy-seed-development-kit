#include "TabPageItem.h"
#include "Page.h"
#include "../View/View.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    TabPageItem::TabPageItem(string prmTitle, Page *prmContent)
    {
        title = prmTitle;
        content = prmContent;
    }

    void TabPageItem::Init(string prmTitle, Page *prmContent)
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