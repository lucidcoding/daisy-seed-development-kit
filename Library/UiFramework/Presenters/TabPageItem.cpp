#include "Page.h"
#include "TabPageItem.h"
#include "../Views/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    TabPageItem::TabPageItem(const char *prmTitle, Page *prmContent)
    {
        title = prmTitle;
        content = prmContent;
    }

    void TabPageItem::Init(const char *prmTitle, Page *prmContent)
    {
        title = prmTitle;
        content = prmContent;
    }

    Page *TabPageItem::GetContent()
    {
        return content;
    }

    const char *TabPageItem::GetTitle()
    {
        return title;
    }
}