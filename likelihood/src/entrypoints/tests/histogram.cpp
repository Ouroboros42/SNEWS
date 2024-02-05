#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "histogram.hpp"

TEST_CASE("Histogram construction") {
    Histogram hist(10, 0, 10);

    vec data_a = { 1, 1.2, 1.3 };
    vec data_b = { -30, 2.4, 10, 100 };
    REQUIRE(hist.add_data(data_a) == 3);
    REQUIRE(hist.add_data(data_b) == 2);

    REQUIRE(hist.get_n_data() == 5);

    REQUIRE(hist.get_lo() == 0.);
    REQUIRE(hist.get_hi() == 10.);
    REQUIRE(hist.get_delta() == 1.);

    REQUIRE(hist.max_bin() == 3);

    REQUIRE(hist.get_bin(0) == 0);
    REQUIRE(hist.get_bin(1) == 3);
    REQUIRE(hist.get_bin(2) == 1);
    REQUIRE(hist.get_bin(3) == 0);
    REQUIRE(hist.get_bin(9) == 1);
}