#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../Presenters/PotentiometerArrayPageItem.h"
#include "../../Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../../../Utilities/TestTools/Debug.h"
#include "../../../../../ThirdParty/catch.hpp"

using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tests::unitTests;

TEST_CASE("Calling SetCurrentKnobPosition with default range when syncronised updates display values correctly")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
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

TEST_CASE("Calling SetCurrentKnobPosition with custom positive zero minimum range when syncronised updates display values correctly")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
    item.SetScaledRange(0, 100);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    REQUIRE(item.GetDisplayValue() == 0);
    item.SetCurrentKnobPosition(0.5f);
    REQUIRE(item.GetDisplayValue() == 50);
    item.SetCurrentKnobPosition(0.999f);
    REQUIRE(item.GetDisplayValue() == 100);
}

TEST_CASE("Calling SetCurrentKnobPosition with custom positive non-zero minimum range when syncronised updates display values correctly")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
    item.SetScaledRange(50, 100);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    REQUIRE(item.GetDisplayValue() == 50);
    item.SetCurrentKnobPosition(0.5f);
    REQUIRE(item.GetDisplayValue() == 75);
    item.SetCurrentKnobPosition(0.999f);
    REQUIRE(item.GetDisplayValue() == 100);
}

TEST_CASE("Calling SetCurrentKnobPosition with custom negative minimum range when syncronised updates display values correctly")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
    item.SetScaledRange(-24, 24);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    REQUIRE(item.GetDisplayValue() == -24);
    item.SetCurrentKnobPosition(0.5f);
    REQUIRE(item.GetDisplayValue() == 0);
    item.SetCurrentKnobPosition(0.999f);
    REQUIRE(item.GetDisplayValue() == 24);
}

TEST_CASE("Changing knob position will not change output value in direct mode if not moved enough")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
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
    NumericPotentiometerArrayPageItem item;
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
    NumericPotentiometerArrayPageItem item;
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
    NumericPotentiometerArrayPageItem item;
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
    NumericPotentiometerArrayPageItem item;
    item.SetOutputValue(0.5f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.502f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == true);
}

TEST_CASE("Setting focus when knob position is not near output value registers item as not in sync")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
    item.SetOutputValue(0.5f);
    item.SetFocus();
    item.SetCurrentKnobPosition(0.6f);
    REQUIRE(item.GetKnobPositionAndOutputValueSynced() == false);
}

TEST_CASE("Incrementing value by less than hysteresis band does not increase scaled value")
{
    MockView view;
    NumericPotentiometerArrayPageItem item;
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
    NumericPotentiometerArrayPageItem item;
    item.SetFocus();
    item.SetCurrentKnobPosition(0.0f);
    item.SetCurrentKnobPosition(0.0390625f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.0388625f);
    REQUIRE(item.GetDisplayValue() == 10);
    item.SetCurrentKnobPosition(0.0380625f);
    REQUIRE(item.GetDisplayValue() == 9);
}