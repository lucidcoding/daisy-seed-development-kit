#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Utilities/UiParameter.h"
#include "../../Utilities/UiParameterProvider.h"

using namespace developmentKit::library::uiFramework::tabs::tests;
using namespace developmentKit::library::uiFramework::tabs::utilities;

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
