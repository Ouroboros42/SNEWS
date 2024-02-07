#include "terms.hpp"
#include "converging.hpp"

#include <iostream>
#include <cmath>

BinLikelihoodCache::BinLikelihoodCache(
    size_t max_count_1, size_t max_count_2,
    scalar background_rate_1, scalar background_rate_2,
    scalar sensitivity_1_over_tot // a / (a + p)
) : FactorialCache(max_count_1 + max_count_2 + 1),
max_count_1(max_count_1), max_count_2(max_count_2),
background_rate_1(background_rate_1), background_rate_2(background_rate_2),
log_event_likelihood_1(std::log(sensitivity_1_over_tot)), log_event_likelihood_2(std::log(1 - sensitivity_1_over_tot)),
log_rate_const_1(std::log(background_rate_1 / sensitivity_1_over_tot)),
log_rate_const_2(std::log(background_rate_2 / (1 - sensitivity_1_over_tot)))
{
    

}

std::function<scalar(size_t i, size_t j)> BinLikelihoodCache::log_sum_terms(size_t count_1, size_t count_2) {
    return [this, count_1, count_2](size_t i, size_t j) {
        return log_exp_series_term(log_rate_const_1, i) + log_exp_series_term(log_rate_const_2, j) + log_binomial(count_1 - i, count_2 - j);
    };
}

scalar BinLikelihoodCache::log_converging_internal_likelihood(size_t count_1, size_t count_2, scalar log_accuracy) {
    // Scale termwise accuracy to number of terms to achieve reliable overall accuracy
    return log_converging_double_sum(count_1, count_2, log_sum_terms(count_1, count_2), log_accuracy - log(count_1) - log(count_2));
}

scalar BinLikelihoodCache::log_likelihood(Histogram time_dist_1, Histogram time_dist_2, size_t n_bins, scalar log_accuracy) {
    scalar log_bin_accuracy = log_accuracy; // TODO Identify correct error propagation

    scalar total = log_event_likelihood_1 * time_dist_1.get_n_data() + log_event_likelihood_2 * time_dist_2.size();

    for (size_t i = 0; i < n_bins; i++) {
        total += log_converging_internal_likelihood(time_dist_1.get_bin(i), time_dist_2.get_bin(i), log_bin_accuracy);
    }

    return total;
}