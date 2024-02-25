#include "../Presenters/Root.h"
#include "NavigationListPageItem.h"
#include <string>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;
    using namespace developmentKit::library::uiFramework;

    void NavigationListPageItem::Init(string prmTitle, Page *prmNext, Root *prmRoot)
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

    string NavigationListPageItem::GetTitle()
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