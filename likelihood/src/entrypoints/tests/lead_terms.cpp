#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "detector_info/relation.hpp"
#include "sum_terms.hpp"

auto approx(scalar value) {
    return Catch::Approx(value).epsilon(0.00001);
}

void test_row_lead_term(FactorialCache cache, DetectorRelation comp, size_t n, size_t m) {
    BinSumTerms big_bin(cache, comp, n, m);
    
    for (size_t i = 0; i <= n; i++) {
        size_t lead_j = comp.lead_index_2(n, m, i);
        
        INFO("n = " << n << ", m = " << m);
        INFO("i = " << i << ", j = " << lead_j);

        auto lead_term = approx(big_bin.get(i, lead_j));
        
        if (lead_j > 0) {
            CHECK(lead_term >= big_bin.get(i, lead_j - 1));
        }

        if (lead_j < m) {
            CHECK(lead_term >= big_bin.get(i, lead_j + 1));
        }
    }
}

void test_lead_term(FactorialCache cache, DetectorRelation comp, size_t n, size_t m) {
    BinSumTerms big_bin(cache, comp, n, m);

    size_t lead_i = comp.lead_index_1(n, m);
    size_t lead_j = comp.lead_index_2(n, m, lead_i);

    INFO("n = " << n << ", m = " << m);
    INFO("i = " << lead_i << ", j = " << lead_j);

    auto lead_term = approx(big_bin.get(lead_i, lead_j));

    if (lead_i > 0) {
        CHECK(lead_term >= big_bin.get(lead_i - 1, comp.lead_index_2(n, m, lead_i - 1)));
    }

    if (lead_i < n) {
        CHECK(lead_term >= big_bin.get(lead_i + 1, comp.lead_index_2(n, m, lead_i + 1)));
    }

    if (lead_j > 0) {
        CHECK(lead_term >= big_bin.get(lead_i, lead_j - 1));
    }

    if (lead_j < m) {
        CHECK(lead_term >= big_bin.get(lead_i, lead_j + 1));
    }
}

vec backgrounds = {10, 10000}; // {0.1, 1, 10, 100, 1000, 10000};
vec ratios = {1000}; // { 1, 10, 100, 1000 };
std::vector<size_t> event_counts = {10000}; // { 1, 10, 100, 1000, 10000 };

TEST_CASE("Bin likelihood row lead term") {
    FactorialCache cache;

    for (scalar ratio : ratios) {
        for (scalar background_1 : backgrounds) {
            for (scalar background_2 : backgrounds) {
                DetectorRelation detector(background_1, background_2, ratio);

                for (size_t count_1: event_counts) {
                    for (size_t count_2: event_counts) {
                        INFO("Bg 1: " << background_1 << ", 2: " << background_2);
                        INFO("Ratio: " << ratio);

                        test_row_lead_term(cache, detector, count_1, count_2);
                    }
                }
            }
        }
    }
}

TEST_CASE("Bin likelihood lead term") {
    FactorialCache cache;

    for (scalar ratio : ratios) {
        for (scalar background_1 : backgrounds) {
            for (scalar background_2 : backgrounds) {
                DetectorRelation detector(background_1, background_2, ratio);

                for (size_t count_1: event_counts) {
                    for (size_t count_2: event_counts) {
                        INFO("Bg 1: " << background_1 << ", 2: " << background_2);
                        INFO("Ratio: " << ratio);

                        test_lead_term(cache, detector, count_1, count_2);
                    }
                }
            }
        }
    }
}