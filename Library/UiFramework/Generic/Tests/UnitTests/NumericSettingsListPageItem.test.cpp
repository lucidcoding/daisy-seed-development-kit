#include "../../Presenters/NumericSettingsListPageItem.h"
#include "../../../../../ThirdParty/catch.hpp"

using namespace developmentKit::library::uiFramework::presenters;

TEST_CASE("UiParameter returns correct value when 0 within positive range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, 0, 255, 0);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 0.0f);
}

TEST_CASE("UiParameter returns correct value when max value within positive range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, 0, 255, 255);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 1.0f);
}

TEST_CASE("UiParameter returns correct value when half way within positive range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, 0, 100, 50);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 0.5f);
}

TEST_CASE("UiParameter returns correct value when min value within positive or negative range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, -100, 100, -100);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 0.0f);
}

TEST_CASE("UiParameter returns correct value when zero within positive or negative range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, -100, 100, 0);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 0.5f);
}

TEST_CASE("UiParameter returns correct value when max value within positive or negative range")
{
    NumericSettingsListPageItem numericSettingsListPageItem;
    numericSettingsListPageItem.Init("TEST", NULL, -100, 100, 100);
    REQUIRE(numericSettingsListPageItem.GetScaledValue() == 1.0f);
}
