#include "../Presenters/Root.h"
#include "NavigationListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    using namespace developmentKit::library::uiFramework;

    void NavigationListPageItem::Init(const char *prmTitle, Page *prmNext, Root *prmRoot)
    {
        title = prmTitle;
        next = prmNext;
        root = prmRoot;
    }

    void NavigationListPageItem::Increment()
    {
    }

    void NavigationListPageItem::Decrement()
    {
    }

    const char *NavigationListPageItem::GetTitle()
    {
        return title;
    }

    void NavigationListPageItem::Select()
    {
        root->SetCurrentPage(next);
    }

    ListPageItem::ListPageItemType NavigationListPageItem::GetType()
    {
        return ListPageItemType::NAVIGATION_PAGE_ITEM;
    }
}