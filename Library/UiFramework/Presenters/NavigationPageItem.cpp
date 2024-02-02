#include "../Display.h"
#include "NavigationPageItem.h"
#include "PageItemType.h"
#include <string>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;
    using namespace developmentKit::library::uiFramework;

    NavigationPageItem::NavigationPageItem(string prmTitle, Page *prmNext, Display *prmRoot)
    {
        title = prmTitle;
        next = prmNext;
        root = prmRoot;
    }

    void NavigationPageItem::Increment()
    {
    }

    void NavigationPageItem::Decrement()
    {
    }

    string NavigationPageItem::GetTitle()
    {
        return title;
    }

    void NavigationPageItem::Select()
    {
        root->SetCurrentPage(next);
    }

    PageItemType NavigationPageItem::GetType()
    {
        return PageItemType::NAVIGATION_PAGE_ITEM;
    }
}