//#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Utilities/PotentiometerToDisplayValueScaler.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../../../Utilities/TestTools/Debug.h"

using namespace developmentKit::library::uiFramework::shared::scaling;

TEST_CASE("Calling Scale on positive scale returns correct values")
{
    PotentiometerToDisplayValueScaler scaler;
    scaler.Init(0, 99);
    REQUIRE(scaler.Scale(0.00005f) == 0);
    REQUIRE(scaler.Scale(0.00999f) == 0);
    REQUIRE(scaler.Scale(0.01f) == 1);
    REQUIRE(scaler.Scale(0.01001f) == 1);
    REQUIRE(scaler.Scale(0.0199f) == 1);
    REQUIRE(scaler.Scale(0.02f) == 2);
    REQUIRE(scaler.Scale(0.99f) == 99);
    REQUIRE(scaler.Scale(0.99979f) == 99);
}

TEST_CASE("Calling Scale on positive short-range scale returns correct values")
{
    PotentiometerToDisplayValueScaler scaler;
    scaler.Init(0, 4);
    REQUIRE(scaler.Scale(0.00005f) == 0);
    REQUIRE(scaler.Scale(0.19999f) == 0);
    REQUIRE(scaler.Scale(0.2f) == 1);
    REQUIRE(scaler.Scale(0.20001f) == 1);
    REQUIRE(scaler.Scale(0.39999f) == 1);
    REQUIRE(scaler.Scale(0.4f) == 2);
    REQUIRE(scaler.Scale(0.8f) == 4);
    REQUIRE(scaler.Scale(0.99979f) == 4);
}

TEST_CASE("Calling Scale on non-zero indexed positive scale returns correct values")
{
    PotentiometerToDisplayValueScaler scaler;
    scaler.Init(100, 199);
    REQUIRE(scaler.Scale(0.00005f) == 100);
    REQUIRE(scaler.Scale(0.01f) == 101);
    REQUIRE(scaler.Scale(0.01001f) == 101);
    REQUIRE(scaler.Scale(0.02f) == 102);
    REQUIRE(scaler.Scale(0.99f) == 199);
    REQUIRE(scaler.Scale(0.99979f) == 199);
}

TEST_CASE("Calling Scale on sweeping scale returns correct values")
{
    PotentiometerToDisplayValueScaler scaler;
    scaler.Init(-2, 2);
    REQUIRE(scaler.Scale(0.00005f) == -2);
    REQUIRE(scaler.Scale(0.19999f) == -2);
    REQUIRE(scaler.Scale(0.2f) == -1);
    REQUIRE(scaler.Scale(0.20001f) == -1);
    REQUIRE(scaler.Scale(0.39999f) == -1);
    REQUIRE(scaler.Scale(0.4f) == 0);
    REQUIRE(scaler.Scale(0.40001f) == 0);
    REQUIRE(scaler.Scale(0.59999f) == 0);
    REQUIRE(scaler.Scale(0.6f) == 1);
    REQUIRE(scaler.Scale(0.8f) == 2);
    REQUIRE(scaler.Scale(0.99979f) == 2);
}