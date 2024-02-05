#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "histogram.hpp"

TEST_CASE("Histogram construction") {
    Histogram hist(10, 0, 10);

    vec data_a = { 1, 1.2, 1.3 };
    vec data_b = { -30, 2.4, 10, 100 };
    REQUIRE(hist.addData(data_a) == 3);
    REQUIRE(hist.addData(data_b) == 2);

    REQUIRE(hist.getNData() == 5);

    REQUIRE(hist.getLo() == 0.);
    REQUIRE(hist.getHi() == 10.);
    REQUIRE(hist.getDelta() == 1.);
    
    REQUIRE(hist.getBin(0) == 0);
    REQUIRE(hist.getBin(1) == 3);
    REQUIRE(hist.getBin(2) == 1);
    REQUIRE(hist.getBin(3) == 0);
    REQUIRE(hist.getBin(9) == 1);
}