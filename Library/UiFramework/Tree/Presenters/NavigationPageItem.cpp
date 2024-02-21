#include "../Display.h"
#include "NavigationPageItem.h"
#include <string>

namespace developmentKit::library::uiFramework::tree::presenters
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::tree;

    NavigationPageItem::NavigationPageItem(string prmTitle, Page *prmNext, Display *prmRoot)
    {
        title = prmTitle;
        next = prmNext;
        root = prmRoot;
    }

    void NavigationPageItem::Init(string prmTitle, Page *prmNext, Display *prmRoot)
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

    PageItem::PageItemType NavigationPageItem::GetType()
    {
        return PageItemType::NAVIGATION_PAGE_ITEM;
    }
}