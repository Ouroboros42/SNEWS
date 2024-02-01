#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "Catch2/catch.hpp"
#include "../common.hpp"

#include <iostream>
#include <cmath>

TEST_CASE ("Benchmarks") {
    BENCHMARK("Big Pascal") {
        size_t n = 400, m = 30000;

        mat rhombus = pascal_rhombus(n, m);
        for (size_t i = 0; i < m; i++) {
            if (isinf(rhombus[n-1][i])) {
                cout << "\nBreaks at " << (n - 1) << "C" << i;
                break;
            }
        }
        return rhombus;
    };
}