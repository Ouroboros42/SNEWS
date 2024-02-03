#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include "converging.hpp"

TEST_CASE("Converging exponentials") {
    size_t n = 100, m = 100;
    scalar a = 3, b = 5;

    FactorialCache cache(max(n, m)); 

    mat zeros(n, vec(m, 0));
    vec e1 = cache.exp_series(a, n), 
        e2 = cache.exp_series(b, m);
    auto log_product_terms = [e1, e2](size_t i, size_t j) { return e1[i] + e2[j]; }; // log of terms in product of (e ^ a)(e ^ b)

    scalar epsilon = 0.1;
    scalar log_total = log_converging_double_sum(n, m, log_product_terms, epsilon); // Approximately log(e^a * e^b) = a + b 

    REQUIRE_THAT(log_total, Catch::Matchers::WithinRel(a + b, epsilon));
}

TEST_CASE("Bin likelihood bounds") {
    size_t n_max = 100, m_max = 200;
    scalar x = 0.1, y = 0.5;

    BinLikelihoodCache cache(n_max, m_max, x, y);

    auto big_bin = cache.log_sum_terms(n_max, m_max);

    // Terms should be decreasing strongly
    REQUIRE(big_bin(0, 0) > big_bin(n_max / 2, m_max / 2));
    REQUIRE(big_bin(n_max / 2, m_max / 2) > big_bin(n_max, m_max));
}