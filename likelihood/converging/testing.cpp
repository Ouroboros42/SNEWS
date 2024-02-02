#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include "converging.hpp"

TEST_CASE("Converging exponentials") {
    size_t n = 100, m = 100;
    scalar a = 3, b = 5;

    FactorialCache cache(max(n, m)); 

    mat zeros(n, vec(m, 0));
    vec e1 = cache.exp_series(a), e2 = cache.exp_series(b);

    scalar eps = 0.1;

    REQUIRE_THAT(
        log_converging_double_sum(n, m, [e1, e2](size_t i, size_t j) { return e1[i] + e2[j]; }, eps), 
        Catch::Matchers::WithinRel(a + b, eps)
    );
}