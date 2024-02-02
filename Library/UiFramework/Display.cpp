#include "Display.h"
#include "./Model/Parameters.h"
#include "./Presenters/PageItemType.h"
#include "./Presenters/NavigationPageItem.h"
#include "./Presenters/NumericSettingsPageItem.h"
#include "./Presenters/OptionsSettingsPageItem.h"
#include "./Presenters/Option.h"
#include "./Presenters/ListPage.h"
#include "./View/ListPageView.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

namespace developmentKit::library::uiFramework
{
    using namespace developmentKit::library::uiFramework::model;
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace daisy;

    Display::Display(OledDisplay<SSD130xI2c128x64Driver> *display, Parameters *prmParameters)
    {
        ListPageView *listPageView = new ListPageView(display);
        parameters = prmParameters;
        ListPage *home = new ListPage(listPageView);
        ListPage *settings = new ListPage(listPageView);
        settings->AddItem(new NavigationPageItem("Back...", home, this));
        settings->AddItem(new NumericSettingsPageItem("Test1", settings));
        settings->AddItem(new NumericSettingsPageItem("Test2", settings));
        settings->AddItem(new NumericSettingsPageItem("Test3", settings));
        OptionsSettingsPageItem *optionsSettingsPageItem = new OptionsSettingsPageItem("Test4", settings);
        optionsSettingsPageItem->AddOption("Opt1", 1);
        optionsSettingsPageItem->AddOption("Opt2", 2);
        optionsSettingsPageItem->AddOption("Opt3", 3);
        settings->AddItem(optionsSettingsPageItem);
        settings->AddItem(new NumericSettingsPageItem("Test5", settings));
        settings->AddItem(new NumericSettingsPageItem("Test6", settings));
        home->AddItem(new NavigationPageItem("Global settings...", settings, this));
        home->AddItem(new NavigationPageItem("Globals...", settings, this));
        currentPage = home;
    }

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

    Page *Display::GetCurrentPage()
    {
        return currentPage;
    }

    void Display::SetCurrentPage(Page *newCurrentPage)
    {
        currentPage = newCurrentPage;
    }

    void Display::Paint()
    {
        currentPage->Paint();
    }
}