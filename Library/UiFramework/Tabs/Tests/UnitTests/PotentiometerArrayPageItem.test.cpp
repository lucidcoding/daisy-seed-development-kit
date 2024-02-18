#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Display.h"
#include "../../Presenters/PotentiometerArrayPageItem.h"

using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::presenters;
using namespace developmentKit::library::uiFramework::tabs::tests;

TEST_CASE("Calling SetPotentiometerValues with default range when syncronised updates display values correctly")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    REQUIRE(item.GetDisplayValue() == 0);
    item.SetCurrentKnobPosition(0.5f);
    REQUIRE(item.GetDisplayValue() == 128);
    item.SetCurrentKnobPosition(0.999f);
    REQUIRE(item.GetDisplayValue() == 255);
    item.SetCurrentKnobPosition(0.0f);
    item.SetCurrentKnobPosition(1.0f);
    REQUIRE(item.GetDisplayValue() == 256);
}

TEST_CASE("PotentiometerArrayPageItem - Calling SetFocus() record initial values values")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetFocus();
    item.SetCurrentKnobPosition(0.1f);
    REQUIRE(item.GetInitialKnobPosition() == 0.1f);
    item.SetCurrentKnobPosition(0.2f);
    REQUIRE(item.GetInitialKnobPosition() == 0.1f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.3f);
    REQUIRE(item.GetInitialKnobPosition() == 0.3f);
}

TEST_CASE("Changing knob position will not change output value in direct mode if not moved enough")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetOutputValue(0.1f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.5f);
    item.SetCurrentKnobPosition(0.502f);
    REQUIRE(item.GetOutputValue() == 0.1f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == false);
}

TEST_CASE("Changing knob position will change output value in direct mode if moved enough")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetOutputValue(0.1f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.5f);
    item.SetCurrentKnobPosition(0.505f);
    REQUIRE(item.GetOutputValue() == 0.505f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == true);
}

TEST_CASE("Changing knob position will not change output value in catch mode if not moved past inital value")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetKnobMode(PotentiometerArrayPageItem::CATCH);
    item.SetOutputValue(0.1f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.5f);
    item.SetCurrentKnobPosition(0.15f);
    REQUIRE(item.GetOutputValue() == 0.1f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == false);
}

TEST_CASE("Changing knob position will change output value in catch mode if moved past inital value")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetKnobMode(PotentiometerArrayPageItem::CATCH);
    item.SetOutputValue(0.1f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.5f);
    item.SetCurrentKnobPosition(0.05f);
    REQUIRE(item.GetOutputValue() == 0.05f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == true);
}

TEST_CASE("Setting focus when knob position is near output value registers item as in sync")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetOutputValue(0.5f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.502f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == true);
}

TEST_CASE("Setting focus when knob position is not near output value registers item as not in sync")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetOutputValue(0.5f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.6f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == false);
}

TEST_CASE("Incrementing value by less than hysteresis band does not increase scaled value")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    item.SetCurrentKnobPosition(0.0390625f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.04296875f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.04396875f);
    REQUIRE(item.GetDisplayValue() == 11);
}

TEST_CASE("Decrementing value by less than hysteresis band does not decrease scaled value")
{
    MockView view;
    PotentiometerArrayPageItem item;
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    item.SetCurrentKnobPosition(0.0390625f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.0388625f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.0380625f);
    REQUIRE(item.GetDisplayValue() == 9);
}