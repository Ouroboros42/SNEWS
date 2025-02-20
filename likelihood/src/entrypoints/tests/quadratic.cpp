#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "detector_info/relation.hpp"

auto approx_match(scalar target) {
    return Catch::Matchers::WithinRel(target , 0.001);
}

void test_quadratic(scalar root_1, scalar root_2, scalar a) {
    scalar b = - a * (root_1 + root_2);
    scalar c = a * root_1 * root_2;

    scalar low_root = std::min(root_1, root_2);

    scalar solution = quadratic_low_root(a, b, c);

    CHECK_THAT(solution, approx_match(low_root));
}

TEST_CASE("Quadratic solutions") {
    test_quadratic(4.4, 10.2, 3.1);
    test_quadratic(-2.2, 3.7, -0.1);
    test_quadratic(1., 500.3, 30.);
}