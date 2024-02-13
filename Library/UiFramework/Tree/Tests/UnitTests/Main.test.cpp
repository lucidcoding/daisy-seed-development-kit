#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../../../ThirdParty/catch.hpp"
#include "../../Display.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../Utilities/UiParameterProvider.h"

using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::presenters;
using namespace developmentKit::library::uiFramework::tree::tests;
using namespace developmentKit::library::uiFramework::tree::utilities;

TEST_CASE("UiParameter returns correct value when 0 within positive range")
{
    MockUiParameterProvider uiParameterProvider(0, 255, 0);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 0.0f);
}

TEST_CASE("UiParameter returns correct value when max value within positive range")
{
    MockUiParameterProvider uiParameterProvider(0, 255, 255);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 1.0f);
}

TEST_CASE("UiParameter returns correct value when half way within positive range")
{
    MockUiParameterProvider uiParameterProvider(0, 100, 50);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 0.5f);
}

TEST_CASE("UiParameter returns correct value when min value within positive or negative range")
{
    MockUiParameterProvider uiParameterProvider(-100, 100, -100);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 0.0f);
}

TEST_CASE("UiParameter returns correct value when zero within positive or negative range")
{
    MockUiParameterProvider uiParameterProvider(-100, 100, 0);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 0.5f);
}

TEST_CASE("UiParameter returns correct value when max value within positive or negative range")
{
    MockUiParameterProvider uiParameterProvider(-100, 100, 100);
    UiParameter uiParameter;
    uiParameter.Init(&uiParameterProvider, 0.0f, 1.0f, UiParameter::LINEAR);
    REQUIRE(uiParameter.Process() == 1.0f);
}

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