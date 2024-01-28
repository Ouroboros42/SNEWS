#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common.hpp"
#include "histogram.hpp"

#include<iostream>

TEST_CASE("nCr known values") {
    REQUIRE(combinations(5, 3) == 10.);
    REQUIRE(combinations(8, 6) == 28.);
    REQUIRE(combinations_split(5, 3) == 56.);
    REQUIRE(combinations_split(8, 6) == 3003.);
}

TEST_CASE("Histogram construction") {
    Histogram hist = Histogram<10>(0., 10., vector<float>{1., 1.2, 1.3, 2.4, 10.});
    REQUIRE(hist.getLo() == 0.);
    REQUIRE(hist.getHi() == 10.);
    REQUIRE(hist.getDelta() == 1.);
    
    REQUIRE(hist.getBins()[0] == 0);
    REQUIRE(hist.getBins()[1] == 3);
    REQUIRE(hist.getBins()[2] == 1);
    REQUIRE(hist.getBins()[3] == 0);
    // ...
    REQUIRE(hist.getBins()[9] == 1);
}