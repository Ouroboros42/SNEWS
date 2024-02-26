#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "detector_info/relation.hpp"
#include "caching/factorials.hpp"
#include "fast_sum/sum_terms.hpp"

void test_flip(FactorialCache& cache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision) {
    scalar unflipped_result;
    scalar flipped_result;

    cache.build_upto(count_1 + count_2);

    BENCHMARK("Unflipped") {
        unflipped_result = bin_log_likelihood_unscaled(cache, detectors, count_1, count_2, rel_precision);
    };
    BENCHMARK("Flipped") {
        DetectorRelation flipped_dets = detectors.flip();
        flipped_result = bin_log_likelihood_unscaled(cache, flipped_dets, count_2, count_1, rel_precision);  
    };

    CHECK_THAT(flipped_result, Catch::Matchers::WithinRel(unflipped_result, rel_precision));
}

TEST_CASE("Transpose speed comparison") {
    DetectorRelation detectors(10000, 1, 0.01);
    FactorialCache cache;

    test_flip(cache, detectors, 10100, 2, 1e-5);
}