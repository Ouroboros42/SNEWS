#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "caching/factorials.hpp"

#include <cmath>
#include <numeric>

auto approx_match(scalar target) {
    return Catch::Matchers::WithinRel(target , 0.001);
}

scalar std_log_factorial(scalar n) {
    return lgamma(n + 1);
}

scalar std_binomial(scalar n, scalar r) {
    return 1/((n+1)*std::beta(n-r+1, r+1));
}

scalar std_log_binomial(scalar n, scalar r) {
    return std_log_factorial(n) - std_log_factorial(n - r) - std_log_factorial(r);
}

TEST_CASE("True nCr known values") {
    CHECK_THAT(10., approx_match(std_binomial(5, 3)));
    CHECK_THAT(48620., approx_match(std_binomial(18, 9))); 

    CHECK_THAT(log(10.), approx_match(std_log_binomial(5, 3)));
    CHECK_THAT(log(48620.), approx_match(std_log_binomial(18, 9)));
}

TEST_CASE("Log factorial cache accuracy") {
    size_t n = 50;

    auto cache = FactorialCache(n);
    for (size_t i = 1; i <= n; i++) {
        CHECK_THAT(cache.log(i), approx_match(log(i)));
        CHECK_THAT(cache.log_factorial(i), approx_match(std_log_factorial(i)));
    }

    scalar x = 5;

    vec exp_x_series = cache.exp_series(x, n-1);
    scalar tot = 0;
    for (scalar term : exp_x_series) {
        tot += exp(term);
    }

    CHECK_THAT(tot, approx_match(exp(x)));
}

TEST_CASE("log binomial cache test") {
    size_t n = 100;
    FactorialCache cache(n);

    for (size_t t = 0; t <= n; t++) {
        for (size_t r = 0; r <= t; r++) {
            CHECK_THAT(cache.log_binomial(t-r, r), approx_match(std_log_binomial(t, r)));
        }
    }
}