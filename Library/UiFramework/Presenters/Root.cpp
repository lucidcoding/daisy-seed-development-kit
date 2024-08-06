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

    void Root::SetPotentiometerValues(float *values)
    {
        currentPage->SetPotentiometerValues(values);
    }

    void Root::Up()
    {
        currentPage->Up();
    }

    void Root::Left()
    {
        currentPage->Left();
    }

    void Root::Right()
    {
        currentPage->Right();
    }

    void Root::Down()
    {
        currentPage->Down();
    }

    void Root::Back()
    {
        currentPage->Back();
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