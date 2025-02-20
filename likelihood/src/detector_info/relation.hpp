#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "data_io/histogram.hpp"
#include "fast_sum/converging.hpp"
#include "caching/factorials.hpp"
#include "fast_sum/lazy.hpp"
#include "data_io/read_signal.hpp"

#include <functional>

typedef std::function<scalar(size_t i, size_t j)> term_gen;

/* Encodes information about relative rates of both detectors
Relative sensitivity to supernova event is inferred from number of observed events */
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
    
        DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1);

        DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_1, scalar sensitivity_2);

        DetectorRelation(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2);
        
        DetectorRelation(scalar background_rate_1, scalar background_rate_2, TimeSeries signal_1, TimeSeries signal_2, scalar bin_width);
        
        DetectorRelation(scalar background_rate_1, scalar background_rate_2, Histogram signal_1, TimeSeries signal_2);
        
        scalar log_likelihood_prefactor(size_t total_events_1, size_t total_events_2);

        size_t lead_index_1(size_t count_1, size_t count_2);

        size_t lead_index_2(size_t count_1, size_t count_2, size_t index_1) const;
};

// For a = 1
scalar quadratic_low_root(scalar b, scalar c);

// Assumes real roots
scalar quadratic_low_root(scalar a, scalar b, scalar c);

#endif