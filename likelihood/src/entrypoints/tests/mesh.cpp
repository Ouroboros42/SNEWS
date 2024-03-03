#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "mesh.hpp"

mesh likelihoods = { {0, 20}, {1, 5}, {2, 10}, {3, -1}, {4, 5}, {5, -3} };

TEST_CASE("Extrema in range") {
    auto [min, max] = min_max_likelihood_in_range(likelihoods, 1, 4);
    
    CHECK(min == -1);
    CHECK(max == 10);

    CHECK_THROWS(min_max_likelihood_in_range(likelihoods, -2, -1));
}

TEST_CASE("Range over threshold") {
    auto [min, max] = range_over_likelihood_threshold(likelihoods, 0);

    CHECK(min == 0);
    CHECK(max == 4);

    CHECK_THROWS(range_over_likelihood_threshold(likelihoods, 21));
}