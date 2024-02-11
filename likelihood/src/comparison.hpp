#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "histogram.hpp"
#include "caching/factorials.hpp"

#include <functional>

// For a = 1
scalar quadratic_low_root(scalar b, scalar c);

// Assumes real roots
scalar quadratic_low_root(scalar a, scalar b, scalar c);

struct DetectorComparison {
    public:
        // -log(1 + p/a)
        scalar log_event_likelihood_1;
        // -log(1 + a/p)
        scalar log_event_likelihood_2;

        // log(b(1 + p/a)) = log(alpha)       
        scalar log_rate_const_1;

        // log(q(1 + a/p)) = log(rho)
        scalar log_rate_const_2;
    
        DetectorComparison(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_ratio_2_to_1);

        DetectorComparison(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2);

        scalar log_likelihood_prefactor(size_t total_events_1, size_t total_events_2);

        size_t lead_index_1(size_t count_1, size_t count_2);

        size_t lead_index_2(size_t count_1, size_t count_2, size_t index_1);
};

std::function<scalar(size_t i, size_t j)> log_sum_terms(FactorialCache cache, DetectorComparison comp, size_t count_1, size_t count_2);

#endif