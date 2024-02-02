#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "../common.hpp"
#include "../histogram.hpp"

#include <iostream>
#include <cmath>
#include <numeric>

float true_ncr(scalar n, scalar r) {
    return 1/((n+1)*beta(n-r+1, r+1)); // tgamma(n + 1) / (tgamma(n - r + 1) * tgamma(r + 1));
}

float log_true_factorial(scalar n) {
    return lgamma(n + 1);
}

float log_true_ncr(scalar n, scalar r) {
    return log_true_factorial(n) - log_true_factorial(n - r) - log_true_factorial(r);
}


auto approx_match(scalar target) {
    return Catch::Matchers::WithinRel(target , 0.001);
}

void print_2D(mat arr) {
    for (vec row : arr) {
        for (scalar elem : row) {
            cout << elem << " ";
        }
        cout << "\n";
    }
}

TEST_CASE("True nCr known values") {
    REQUIRE_THAT(10., approx_match(true_ncr(5, 3)));
    REQUIRE_THAT(48620., approx_match(true_ncr(18, 9))); 

    REQUIRE_THAT(log(10.), approx_match(log_true_ncr(5, 3)));
    REQUIRE_THAT(log(48620.), approx_match(log_true_ncr(18, 9)));
}

TEST_CASE("Single-loop nCr all low values") {
    int N = 100;

    for (int n = 0; n < N; n++) {
        for (int r = 0; r <= n; r++) {
            INFO(n << "C" << r);
            REQUIRE_THAT(combinations(n, r), approx_match(true_ncr(n, r)));
        }
    }
}

TEST_CASE("Pascal triangle accuracy") {
    size_t n = 10, m = 20;

    auto rhombus = pascal_rhombus(n, m);
    
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < m; x++) {
            INFO((y + x) << "C" << x);
            REQUIRE_THAT(rhombus[y][x], approx_match(true_ncr(y + x, x)));
        }
    }
}

TEST_CASE("Scaled pascal triangle accuracy") {
    size_t n = 500, m = 400;
    scalar a = 0.5, b = 0.3;

    auto rhombus = scaled_rhombus(n, m, a, b);
    
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < m; x++) {
            INFO((y + x) << "C" << x);
            REQUIRE_THAT(log(rhombus[y][x]), approx_match(log_true_ncr(y + x, x) + y * log(a) + x * log(b)));
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

TEST_CASE("Log factorial cache accuracy") {
    size_t n = 50;

    auto cache = FactorialCache(n);
    for (size_t i = 1; i <= n; i++) {
        REQUIRE_THAT(cache.log(i), approx_match(log(i)));
        REQUIRE_THAT(cache.log_factorial(i), approx_match(log_true_factorial(i)));
    }

    scalar x = 5;

    vec exp_x_series = cache.exp_series(x);
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
            REQUIRE_THAT(cache.log_binomial(t-r, r), approx_match(log_true_ncr(t, r)));
        }
    }
}