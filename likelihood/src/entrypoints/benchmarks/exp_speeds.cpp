#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <fastexp.hpp>

#include <cmath>

TEST_CASE("Speed comparison") {
    std::vector<double> X = { 1.13, 4.75, 10.40, 200.39, -100.35, -55.61 };

    for (double x: X) {
        std::string x_brackets = "(" + std::to_string(x) + ")";

        double expected, estimate;

        BENCHMARK("True Exp" + x_brackets) {
            expected = std::exp(x);
        };

        BENCHMARK("Fast Exp" + x_brackets) {
            estimate = fastExp(x);
        };

        CHECK_THAT(estimate, Catch::Matchers::WithinRel(expected, 0.03));
    }
}