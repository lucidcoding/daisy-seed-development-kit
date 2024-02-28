#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "TestTreeRoot.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Presenters/TreeRoot.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../Utilities/UiParameterProvider.h"

using namespace developmentKit::library::uiFramework::tree::presenters;
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

// ---- EVERYTHING BELOW COVERED IN ListPage.test.cpp
