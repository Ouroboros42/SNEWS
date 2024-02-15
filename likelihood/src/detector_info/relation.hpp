#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "histogram.hpp"
#include "caching/factorials.hpp"

#include <functional>

struct DetectorRelation {
    public:
        // -log(1 + p/a)
        scalar log_sensitivity_1;
        // -log(1 + a/p)
        scalar log_sensitivity_2;
        
        // b(1 + p/a) = alpha
        scalar rate_const_1;  
        scalar log_rate_const_1;

        // q(1 + a/p) = rho
        scalar rate_const_2;
        scalar log_rate_const_2;

        scalar rate_const_ratio_2_to_1;
    
        DetectorRelation(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_ratio_2_to_1);

        DetectorRelation(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_1, scalar sensitivity_2);

        DetectorRelation(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2);

        scalar log_likelihood_prefactor(size_t total_events_1, size_t total_events_2);

        size_t lead_index_1(size_t count_1, size_t count_2);

        size_t lead_index_2(size_t count_1, size_t count_2, size_t index_1) const;

        std::function<size_t(size_t index_1)> lead_index_2_getter(size_t count_1, size_t count_2);
};

// For a = 1
scalar quadratic_low_root(scalar b, scalar c);

// Assumes real roots
scalar quadratic_low_root(scalar a, scalar b, scalar c);

std::function<scalar(size_t i, size_t j)> log_sum_terms(FactorialCache cache, DetectorRelation comp, size_t count_1, size_t count_2);

#endif