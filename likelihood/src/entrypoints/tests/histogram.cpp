#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "histogram.hpp"

TEST_CASE("Histogram construction") {
    Histogram hist(10, 0, 10);

    vec data_a = { 1, 1.2, 1.3 };
    vec data_b = { -30, 2.4, 10, 100 };
    CHECK(hist.add_data(data_a) == 3);
    CHECK(hist.add_data(data_b) == 2);

    CHECK(hist.n_data() == 5);

    CHECK(hist.lo() == 0.);
    CHECK(hist.hi() == 10.);
    CHECK(hist.delta() == 1.);

    CHECK(hist.max_bin() == 3);

    CHECK(hist[0] == 0);
    CHECK(hist[1] == 3);
    CHECK(hist[2] == 1);
    CHECK(hist[3] == 0);
    CHECK(hist[9] == 1);
}