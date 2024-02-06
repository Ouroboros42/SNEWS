#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "surplus/binomials.hpp"
#include "caching/factorials.hpp"

#include <cmath>
#include <numeric>

auto approx_match(scalar target) {
    return Catch::Matchers::WithinRel(target , 0.001);
}

TEST_CASE("True nCr known values") {
    REQUIRE_THAT(10., approx_match(std_binomial(5, 3)));
    REQUIRE_THAT(48620., approx_match(std_binomial(18, 9))); 

    REQUIRE_THAT(log(10.), approx_match(std_log_binomial(5, 3)));
    REQUIRE_THAT(log(48620.), approx_match(std_log_binomial(18, 9)));
}

TEST_CASE("Single-loop nCr all low values") {
    int N = 100;

    for (int n = 0; n < N; n++) {
        for (int r = 0; r <= n; r++) {
            INFO(n << "C" << r);
            REQUIRE_THAT(binomial(n, r), approx_match(std_binomial(n, r)));
        }
    }
}

TEST_CASE("Log factorial cache accuracy") {
    size_t n = 50;

    auto cache = FactorialCache(n);
    for (size_t i = 1; i <= n; i++) {
        REQUIRE_THAT(cache.log(i), approx_match(log(i)));
        REQUIRE_THAT(cache.log_factorial(i), approx_match(std_log_factorial(i)));
    }

    scalar x = 5;

    vec exp_x_series = cache.exp_series(x, n-1);
    scalar tot = 0;
    for (scalar term : exp_x_series) {
        tot += exp(term);
    }

    REQUIRE_THAT(tot, approx_match(exp(x)));
}

TEST_CASE("log binomial cache test") {
    size_t n = 100;
    FactorialCache cache(n);

    for (size_t t = 0; t <= n; t++) {
        for (size_t r = 0; r <= t; r++) {
            REQUIRE_THAT(cache.log_binomial(t-r, r), approx_match(std_log_binomial(t, r)));
        }
    }
}