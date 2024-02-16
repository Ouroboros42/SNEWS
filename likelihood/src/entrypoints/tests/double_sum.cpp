#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "converging.hpp"
#include "sum_terms.hpp"

scalar add_all_log_terms(Lazy2DArray& log_terms) {
    scalar log_total = log_terms.get(0, 0);

    for(size_t i = 0; i < log_terms.size_x(); i++) {
        for (size_t j = i ? 0 : 1; j < log_terms.size_y(); j++) {
            log_total += std::log(1 + std::exp(log_terms.get(i, j) - log_total));
        }
    }

    return log_total;
}

void test_bin(DetectorRelation rel, FactorialCache cache, size_t n, size_t m) {
    BinSumTerms terms(cache, rel, n, m);

    scalar accuracy = 0.00001; 

    INFO("n = " << n << ", m = " << m);
    CHECK_THAT(bin_log_likelihood_unscaled(cache, rel, n, m, accuracy), Catch::Matchers::WithinRel(add_all_log_terms(terms), accuracy * 10));
}

TEST_CASE("Converging bin likelihood test") {
    std::vector<size_t> event_counts = { 1, 10, 100, 1000 };
    vec background_rates = { 0.001, 0.1, 1, 10, 100, 1000 };
    vec sensitivity_ratios = { 1, 10, 100, 1000 };

    FactorialCache cache;

    for (scalar ratio : sensitivity_ratios) {
        for (scalar background_1 : background_rates) {
            for (scalar background_2 : background_rates) {
                DetectorRelation detectors(background_1, background_2, ratio);

                for (size_t count_1: event_counts) {
                    for (size_t count_2: event_counts) {
                        INFO("ratio = " << ratio);
                        INFO("background 1 = " << background_1 << ", 2 = " << background_2);
                        test_bin(detectors, cache, count_1, count_2);
                    }
                }
            }
        }
    }
}