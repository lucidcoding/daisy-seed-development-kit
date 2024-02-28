#include "ListPageRoot.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../../../Utilities/TestTools/Debug.h"

using namespace developmentKit::library::uiFramework::tests::unitTests;

TEST_CASE("Navigating through the Display behaves as expected")
{
    ListPageRoot display;
    display.Init();
    REQUIRE(display.GetCurrentPage() == &(display.homeListPage));
    display.Select();
    REQUIRE(display.GetCurrentPage() == &(display.oscillatorListPage));
    display.Select();
    REQUIRE(display.GetCurrentPage() == &(display.homeListPage));
    display.Increment();
    display.Select();
    REQUIRE(display.GetCurrentPage() == &(display.adsrListPage));

}

TEST_CASE("Display updates numeric values as expected")
{
    ListPageRoot display;
    display.Init();
    display.Select();
    display.Increment();
    display.Select();
    display.Increment();
    display.Increment();
    display.Increment();
    REQUIRE(display.levelSettingsPageItem.GetValue() == 19);
}

TEST_CASE("Display updates option values as expected")
{
    ListPageRoot display;
    display.Init();
    display.Select();
    display.Increment();
    display.Increment();
    display.Select();
    display.Increment();
    display.Increment();
    REQUIRE(display.waveformSettingsPageItem.GetValue() == 2);
}