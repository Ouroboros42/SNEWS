#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "test_data/random.hpp"

TEST_CASE("Uniform consistency test") {
    size_t N = 10000;

    scalar bottom = -10, top = 23.1;
    scalar expected_mean = (top + bottom) / 2;

    scalar total = 0;
    for (size_t i = 0; i < N; i++) {
        total += rand_in_range(bottom, top);
    }
    scalar mean = total / N;

    CHECK(mean == Catch::Approx(expected_mean).epsilon(0.1));
}

TEST_CASE("Poisson constency test") {
    size_t N = 10000;

    scalar rate = 1000;

    scalar total = 0;
    for (size_t i = 0; i < N; i++) {
        total += poisson_trial(rate);
    }
    scalar mean = total / N;

    CHECK(mean == Catch::Approx(rate).epsilon(0.1));
}