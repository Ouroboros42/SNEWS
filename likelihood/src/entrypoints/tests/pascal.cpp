#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "surplus/pascal.hpp"
#include "surplus/binomials.hpp"


// void print_2D(mat arr) {
//     for (vec row : arr) {
//         for (scalar elem : row) {
//             std::cout << elem << " ";
//         }
//         std::cout << "\n";
//     }
// }

auto approx_match(scalar target) {
    return Catch::Matchers::WithinRel(target , 0.001);
}

TEST_CASE("Pascal triangle accuracy") {
    size_t n = 10, m = 20;

    auto rhombus = pascal_rhombus(n, m);
    
    for (size_t y = 0; y < n; y++) {
        for (size_t x = 0; x < m; x++) {
            INFO((y + x) << "C" << x);
            REQUIRE_THAT(rhombus[y][x], approx_match(std_binomial(y + x, x)));
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
            REQUIRE_THAT(log(rhombus[y][x]), approx_match(std_log_binomial(y + x, x) + y * log(a) + x * log(b)));
        }
    }
}