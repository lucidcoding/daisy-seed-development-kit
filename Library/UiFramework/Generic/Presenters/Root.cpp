#include "Root.h"

namespace developmentKit::library::uiFramework::presenters
{
    void Root::Increment()
    {
        currentPage->Increment();
    }

    void Root::Decrement()
    {
        currentPage->Decrement();
    }

    void Root::Select()
    {
        currentPage->Select();
    }

    Page *Root::GetCurrentPage()
    {
        return currentPage;
    }

    void Root::SetCurrentPage(Page *newCurrentPage)
    {
        currentPage = newCurrentPage;
    }

    void Root::SetHomePage(Page *newHomePage)
    {
        homePage = newHomePage;
    }

    void Root::Paint()
    {
        currentPage->Paint();
    }
}