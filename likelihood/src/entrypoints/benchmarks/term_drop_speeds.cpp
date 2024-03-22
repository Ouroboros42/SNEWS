#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "detector_info/relation.hpp"
#include "caching/factorials.hpp"
#include "fast_sum/sum_terms.hpp"

scalar add_all_log_terms(Lazy2DArray& log_terms) {
    scalar log_total = log_terms.get(0, 0);

    for(size_t i = 0; i < log_terms.size_x(); i++) {
        for (size_t j = i ? 0 : 1; j < log_terms.size_y(); j++) {
            log_total += std::log(1 + std::exp(log_terms.get(i, j) - log_total));
        }
    }

    return log_total;
}

void speed_compare(FactorialCache& cache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar precision) {
    BENCHMARK("Drop Terms") {
        return bin_log_likelihood_unscaled(cache, detectors, count_1, count_2, precision);
    };

    BENCHMARK("All Terms") {
        BinSumTerms terms(cache, detectors, count_1, count_2);
        scalar tot = add_all_log_terms(terms);
        return tot;
    };

    BENCHMARK("Zero Precision") {
        return bin_log_likelihood_unscaled(cache, detectors, count_1, count_2, 0);
    };
}

TEST_CASE("Speed compare neglecting terms") {
    DetectorRelation detectors(3000, 10, 0.001);
    FactorialCache cache;

    speed_compare(cache, detectors, 30000, 40, 1e-3);
    speed_compare(cache, detectors, 3000, 10, 1e-3);
    speed_compare(cache, detectors, 1000, 10, 1e-3);
}