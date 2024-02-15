#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "detector_info/relation.hpp"

auto approx(scalar value) {
    return Catch::Approx(value).epsilon(0.00001);
}

void test_row_lead_term(FactorialCache cache, DetectorRelation comp, size_t n, size_t m) {
    cache.build_upto(n + m);
    auto big_bin = log_sum_terms(cache, comp, n, m);
    
    for (size_t i = 0; i <= n; i++) {
        size_t lead_j = comp.lead_index_2(n, m, i);
        
        INFO("n = " << n << ", m = " << m);
        INFO("i = " << i << ", j = " << lead_j);

        auto lead_term = approx(big_bin(i, lead_j));
        
        if (lead_j > 0) {
            CHECK(lead_term >= big_bin(i, lead_j - 1));
        }

        if (lead_j < m) {
            CHECK(lead_term >= big_bin(i, lead_j + 1));
        }
    }
}

void test_lead_term(FactorialCache cache, DetectorRelation comp, size_t n, size_t m) {
    cache.build_upto(n + m);
    auto big_bin = log_sum_terms(cache, comp, n, m);

    size_t lead_i = comp.lead_index_1(n, m);
    size_t lead_j = comp.lead_index_2(n, m, lead_i);

    INFO("n = " << n << ", m = " << m);
    INFO("i = " << lead_i << ", j = " << lead_j);

    auto lead_term = approx(big_bin(lead_i, lead_j));

    if (lead_i > 0) {
        CHECK(lead_term >= big_bin(lead_i - 1, comp.lead_index_2(n, m, lead_i - 1)));
    }

    if (lead_i < n) {
        CHECK(lead_term >= big_bin(lead_i + 1, comp.lead_index_2(n, m, lead_i + 1)));
    }

    if (lead_j > 0) {
        CHECK(lead_term >= big_bin(lead_i, lead_j - 1));
    }

    if (lead_j < m) {
        CHECK(lead_term >= big_bin(lead_i, lead_j + 1));
    }
}


TEST_CASE("Bin likelihood row lead term") {
    scalar b1 = 10000, b2 = 10, ratio = 0.01;

    DetectorRelation comp(b1, b2, ratio);
    FactorialCache cache;

    test_row_lead_term(cache, comp, 100000, 10000);
    test_row_lead_term(cache, comp, 1000, 2000);
    test_row_lead_term(cache, comp, 100000, 3000);
    test_row_lead_term(cache, comp, 200000, 400);
    test_row_lead_term(cache, comp, 30, 2);
    test_row_lead_term(cache, comp, 0, 0);
}

TEST_CASE("Bin likelihood lead term") {
    scalar b1 = 10000, b2 = 10, ratio = 0.01;

    DetectorRelation comp(b1, b2, ratio);
    FactorialCache cache;

    test_lead_term(cache, comp, 100000, 10000);
    test_lead_term(cache, comp, 100, 200);
    test_lead_term(cache, comp, 1000, 2000);
    test_lead_term(cache, comp, 100000, 3000);
    test_lead_term(cache, comp, 200000, 400);
    test_lead_term(cache, comp, 30, 2);
    test_lead_term(cache, comp, 0, 0);
}