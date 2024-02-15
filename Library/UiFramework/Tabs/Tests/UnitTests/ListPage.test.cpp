#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Display.h"
#include "../../Presenters/Page.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"

using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::presenters;
using namespace developmentKit::library::uiFramework::tabs::tests;

Display *display;
ListPage *homeListPage;
ListPage *oscillatorListPage;
NumericSettingsPageItem *levelSettingsPageItem;
NumericSettingsPageItem *noteSettingsPageItem;
OptionsSettingsPageItem *waveformSettingsPageItem;
ListPage *adsrListPage;
NumericSettingsPageItem *attackSettingsPageItem;
NumericSettingsPageItem *decaySettingsPageItem;
NumericSettingsPageItem *sustainSettingsPageItem;
NumericSettingsPageItem *releaseSettingsPageItem;

void SetupDisplay()
{
    display = new Display();
    MockView *homeListPageView = new MockView();
    homeListPage = new ListPage(homeListPageView);

    // Setup oscillator page and add to home page.
    MockView *oscillatorListPageView = new MockView();
    oscillatorListPage = new ListPage(oscillatorListPageView);
    oscillatorListPage->AddItem(new NavigationPageItem("Back...", homeListPage, display));
    levelSettingsPageItem = new NumericSettingsPageItem("Level", oscillatorListPage, 0, 127, 16);
    oscillatorListPage->AddItem(levelSettingsPageItem);
    noteSettingsPageItem = new NumericSettingsPageItem("Note", oscillatorListPage, 0, 127, 64);
    oscillatorListPage->AddItem(noteSettingsPageItem);
    waveformSettingsPageItem = new OptionsSettingsPageItem("Waveform", oscillatorListPage);
    waveformSettingsPageItem->AddOption("Sin", 0);
    waveformSettingsPageItem->AddOption("Tri", 1);
    waveformSettingsPageItem->AddOption("Saw", 2);
    waveformSettingsPageItem->AddOption("Squ", 3);
    oscillatorListPage->AddItem(waveformSettingsPageItem);
    homeListPage->AddItem(new NavigationPageItem("Oscillator...", oscillatorListPage, display));

    // Setup envelope page and add to home page.
    MockView *adsrListPageView = new MockView();
    adsrListPage = new ListPage(adsrListPageView);
    adsrListPage->AddItem(new NavigationPageItem("Back...", homeListPage, display));
    attackSettingsPageItem = new NumericSettingsPageItem("Attack", adsrListPage, 0, 127, 0);
    adsrListPage->AddItem(attackSettingsPageItem);
    decaySettingsPageItem = new NumericSettingsPageItem("Decay", adsrListPage, 0, 127, 32);
    adsrListPage->AddItem(decaySettingsPageItem);
    sustainSettingsPageItem = new NumericSettingsPageItem("Sustain", adsrListPage, 0, 127, 16);
    adsrListPage->AddItem(sustainSettingsPageItem);
    releaseSettingsPageItem = new NumericSettingsPageItem("Release", adsrListPage, 0, 127, 16);
    adsrListPage->AddItem(releaseSettingsPageItem);
    homeListPage->AddItem(new NavigationPageItem("Envelope...", adsrListPage, display));

    // Set display home page and current page.
    display->SetHomePage(homeListPage);
    display->SetCurrentPage(homeListPage);
}

void ClearDownDisplay()
{
    delete releaseSettingsPageItem;
    delete sustainSettingsPageItem;
    delete decaySettingsPageItem;
    delete attackSettingsPageItem;
    delete adsrListPage;
    delete waveformSettingsPageItem;
    delete noteSettingsPageItem;
    delete levelSettingsPageItem;
    delete oscillatorListPage;
    delete homeListPage;
    delete display;
}

TEST_CASE("Navigating through the Display behaves as expected")
{
    SetupDisplay();
    REQUIRE(display->GetCurrentPage() == homeListPage);
    display->Select();
    REQUIRE(display->GetCurrentPage() == oscillatorListPage);
    display->Select();
    REQUIRE(display->GetCurrentPage() == homeListPage);
    display->Increment();
    display->Select();
    REQUIRE(display->GetCurrentPage() == adsrListPage);
    ClearDownDisplay();
}

TEST_CASE("Display updates numeric values as expected")
{
    SetupDisplay();
    display->Select();
    display->Increment();
    display->Select();
    display->Increment();
    display->Increment();
    display->Increment();
    REQUIRE(levelSettingsPageItem->GetValue() == 19);
    ClearDownDisplay();
}

TEST_CASE("Display updates option values as expected")
{
    SetupDisplay();
    display->Select();
    display->Increment();
    display->Increment();
    display->Increment();
    display->Select();
    display->Increment();
    display->Increment();
    REQUIRE(waveformSettingsPageItem->GetValue() == 2);
    ClearDownDisplay();
}