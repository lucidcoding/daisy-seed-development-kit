#include "Display.h"
#include "./Presenters/NavigationPageItem.h"
#include "./Presenters/NumericSettingsPageItem.h"
#include "./Presenters/OptionsSettingsPageItem.h"
#include "./Presenters/Option.h"
#include "./Presenters/ListPage.h"

namespace developmentKit::library::uiFramework::tabs
{
    using namespace developmentKit::library::uiFramework::tabs::presenters;

    void Display::Increment()
    {
        currentPage->Increment();
    }

    void Display::Decrement()
    {
        currentPage->Decrement();
    }

    void Display::Select()
    {
        currentPage->Select();
    }

    void Display::SetPotentiometerValues(float *values)
    {
        currentPage->SetPotentiometerValues(values);
    }

    Page *Display::GetCurrentPage()
    {
        return currentPage;
    }

    void Display::SetCurrentPage(Page *newCurrentPage)
    {
        currentPage = newCurrentPage;
    }

    void Display::SetHomePage(Page *newHomePage)
    {
        homePage = newHomePage;
    }

    void Display::Paint()
    {
        currentPage->Paint();
    }
}