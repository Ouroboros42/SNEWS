#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "Catch2/catch.hpp"
#include "../common.hpp"

#include <iostream>
#include <cmath>

TEST_CASE ("Benchmarks") {
    size_t n = 400, m = 30000;

    size_t break_index = m;

    BENCHMARK("Big Pascal") {
        mat rhombus = pascal_rhombus(n, m);
        for (size_t i = 0; i < m; i++) {
            if (isinf(rhombus[n-1][i])) {
                break_index = min(break_index, i);
                break;
            }
        }
        return rhombus;
    };

    if(break_index < m) {
        cout << "\nBreaks at " << (n - 1) << "C" << break_index << "\n";
    }

    break_index = m;

    BENCHMARK("Big scaled Pascal 0.9*0.9") {
        scalar a = 0.9, b = 0.9;

        mat rhombus = scaled_rhombus(n, m, a, b);
        for (size_t i = 0; i < m; i++) {
            if (isinf(rhombus[n-1][i])) {
                break_index = min(break_index, i);
                break;
            }
        }
        return rhombus;
    };

    if(break_index < m) {
        cout << "\nBreaks at " << (n - 1) << "C" << break_index << "\n";
    }
}