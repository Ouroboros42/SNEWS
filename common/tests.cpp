#define CATCH_CONFIG_MAIN

#include "common.hpp"
#include "Catch2/catch.hpp"

#include<iostream>

TEST_CASE("nCr known values") {
    REQUIRE(combinations(5, 3) == 10.);
    REQUIRE(combinations(8, 6) == 28.);
    REQUIRE(combinations_split(5, 3) == 56.);
    REQUIRE(combinations_split(8, 6) == 3003.);
}
}