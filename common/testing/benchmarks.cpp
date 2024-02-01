#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "Catch2/catch.hpp"
#include "../common.hpp"

#include <iostream>

TEST_CASE ("Benchmarks") {
    BENCHMARK("Big Pascal") {
        vector<vector<float>> rhombus = pascal_rhombus(30000, 400);
        cout << rhombus[29999][399];
        return rhombus;
    };
}