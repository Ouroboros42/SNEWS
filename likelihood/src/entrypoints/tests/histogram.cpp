#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "histogram.hpp"

TEST_CASE("Histogram construction") {
    Histogram<10> hist(0., 10., vector<float>{1., 1.2, 1.3, 2.4, 10.});
    REQUIRE(hist.getLo() == 0.);
    REQUIRE(hist.getHi() == 10.);
    REQUIRE(hist.getDelta() == 1.);
    
    REQUIRE(hist.getBins()[0] == 0);
    REQUIRE(hist.getBins()[1] == 3);
    REQUIRE(hist.getBins()[2] == 1);
    REQUIRE(hist.getBins()[3] == 0);
    // ... zeros
    REQUIRE(hist.getBins()[9] == 1);
}