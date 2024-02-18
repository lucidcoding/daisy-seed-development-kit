#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Display.h"
#include "../../Presenters/PotentiometerArrayPageItem.h"

using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::presenters;
using namespace developmentKit::library::uiFramework::tabs::tests;

TEST_CASE("Calling SetPotentiometerValues when syncronised updates display values correctly")
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

