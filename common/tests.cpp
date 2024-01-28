#define CATCH_CONFIG_MAIN

#include "common.hpp"
#include "Catch2/catch.hpp"

#include<iostream>

TEST_CASE("nCr pre-calculated tests") {
    REQUIRE(choose(5, 3) == 10.);
    REQUIRE(choose(8, 6) == 28.);
}