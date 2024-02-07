#ifndef TERM_CACHE_H
#define TERM_CACHE_H

#include "factorials.hpp"
#include "histogram.hpp"

#include <functional>

class BinLikelihoodCache : public FactorialCache {
    private:
        size_t max_count_1;
        size_t max_count_2;

        scalar background_rate_1;
        scalar background_rate_2;

        // -log(1 + p/a)
        scalar log_event_likelihood_1;
        // -log(1 + a/p)
        scalar log_event_likelihood_2;

        scalar log_rate_const_1;
        scalar log_rate_const_2;
        
    public:
        BinLikelihoodCache(
            scalar background_rate_1, scalar background_rate_2,
            scalar sensitivity_1_over_tot // a / (a + p) = 1 / (1 + p/a)
        );

        BinLikelihoodCache(
            size_t max_count_1, size_t max_count_2,
            scalar background_rate_1, scalar background_rate_2,
            scalar sensitivity_1_over_tot // a / (a + p) = 1 / (1 + p/a)
        );

        void build_upto(size_t new_max_count_1, size_t new_max_count_2);

        std::function<scalar(size_t i, size_t j)> log_sum_terms(size_t count_1, size_t count_2);

        scalar log_converging_internal_likelihood(size_t count_1, size_t count_2, scalar log_accuracy);

        // Ignores constant that depends only on number of bins
        scalar log_likelihood(Histogram time_dist_1, Histogram time_dist_2, size_t n_bins, scalar log_accuracy);
};

#endif
