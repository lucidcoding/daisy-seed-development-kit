#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../../Drivers/StepSequencer.h"

typedef unsigned char uint8_t;

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}


//https://android.googlesource.com/platform/external/catch2/+/HEAD/docs/tutorial.md
//https://github.com/catchorg/Catch2/tree/v2.x?tab=readme-ov-file 
