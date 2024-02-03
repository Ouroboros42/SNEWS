#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "histogram.hpp"

TEST_CASE("Histogram construction") {
    Histogram hist(10, 0., 10., {1., 1.2, 1.3, 2.4, 10.});
    REQUIRE(hist.getLo() == 0.);
    REQUIRE(hist.getHi() == 10.);
    REQUIRE(hist.getDelta() == 1.);
    
    REQUIRE(hist.getBin(0) == 0);
    REQUIRE(hist.getBin(1) == 3);
    REQUIRE(hist.getBin(2) == 1);
    REQUIRE(hist.getBin(3) == 0);
    REQUIRE(hist.getBin(9) == 1);
}