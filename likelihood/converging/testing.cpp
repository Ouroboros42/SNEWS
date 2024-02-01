#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"

#include "converging.hpp"

TEST_CASE("Converging exponentials") {
    size_t n = 10, m = 20;
    scalar a = 3, b = 5;

    mat ones(n, vec(m, 1));
    vec e1 = vec(n), e2 = vec(m);

    e1[0] = e2[0] = 1;
     
}