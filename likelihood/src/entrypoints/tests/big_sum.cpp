#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "converging.hpp"

TEST_CASE("Converging exponentials") {
    size_t n = 100, m = 100;
    scalar a = 3, b = 5;

    FactorialCache cache(std::max(n, m)); 

    mat zeros(n, vec(m, 0));
    vec e1 = cache.exp_series(a, n), 
        e2 = cache.exp_series(b, m);
    auto log_product_terms = [e1, e2](size_t i, size_t j) { return e1[i] + e2[j]; }; // log of terms in product of (e ^ a)(e ^ b)

    scalar epsilon = 0.1;
    scalar log_total = log_converging_double_sum(n, m, log_product_terms, std::log(epsilon / n / m)); // Approximately log(e^a * e^b) = a + b 

    CHECK_THAT(log_total, Catch::Matchers::WithinRel(a + b, epsilon));
}