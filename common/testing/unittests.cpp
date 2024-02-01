#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "../common.hpp"
#include "../histogram.hpp"

#include <iostream>
#include <cmath>

float true_ncr(float n, float r) {
    return 1/((n+1)*beta(n-r+1, r+1)); // tgamma(n + 1) / (tgamma(n - r + 1) * tgamma(r + 1));
}

auto ncr_matcher(float n, float r) {
    return Catch::Matchers::WithinRel(true_ncr(n, r), 0.001f);
}

TEST_CASE("True ncr tests") {
    REQUIRE_THAT(10., ncr_matcher(5, 3));
    REQUIRE_THAT(48620., ncr_matcher(18, 9)); 
}

TEST_CASE("Combinations all low values") {
    int N = 100;

    for (int n = 0; n < N; n++) {
        for (int r = 0; r <= n; r++) {
            INFO(n << "C" << r);
            REQUIRE_THAT(combinations(n, r), ncr_matcher(n, r));
        }
    }
}

TEST_CASE("Pascal Triangle tests") {
    size_t n = 10, m = 20;

    auto rhombus = pascal_rhombus(n, m);
    
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < m; x++) {
            INFO((y + x) << "C" << x);
            REQUIRE_THAT(rhombus[y][x], ncr_matcher(y + x, x));
        }
    }
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