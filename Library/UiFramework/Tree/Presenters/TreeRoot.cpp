#include "TreeRoot.h"
#include "./NavigationPageItem.h"
#include "./NumericSettingsPageItem.h"
#include "./OptionsSettingsPageItem.h"
#include "./Option.h"
#include "./ListPage.h"

namespace developmentKit::library::uiFramework::tree::presenters
{
    void TreeRoot::Increment()
    {
        currentPage->Increment();
    }

    void TreeRoot::Decrement()
    {
        currentPage->Decrement();
    }

    void TreeRoot::Select()
    {
        currentPage->Select();
    }

    Page *TreeRoot::GetCurrentPage()
    {
        return currentPage;
    }

    void TreeRoot::SetCurrentPage(Page *newCurrentPage)
    {
        currentPage = newCurrentPage;
    }

    void TreeRoot::SetHomePage(Page *newHomePage)
    {
        homePage = newHomePage;
    }
}