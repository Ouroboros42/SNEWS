#include "comparison.hpp"

#include <cmath>

DetectorComparison::DetectorComparison(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_ratio_2_to_1) : 
    log_event_likelihood_1(-std::log(1 + sensitivity_ratio_2_to_1)),
    log_event_likelihood_2(-std::log(1 + 1 / sensitivity_ratio_2_to_1)),
    log_rate_const_1(std::log(background_rate_1) - log_event_likelihood_1),
    log_rate_const_2(std::log(background_rate_2) - log_event_likelihood_2) 
{}

DetectorComparison::DetectorComparison(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2)
: DetectorComparison(
    background_rate_1, background_rate_2,
    (events_2.mean_rate() - background_rate_2) / (events_1.mean_rate() - background_rate_1)
) {}

scalar DetectorComparison::log_likelihood_prefactor(size_t total_events_1, size_t total_events_2) {
    return log_event_likelihood_1 * total_events_1 + log_event_likelihood_2 * total_events_2;
}

std::function<scalar(size_t i, size_t j)> log_sum_terms(FactorialCache cache, DetectorComparison comp, size_t count_1, size_t count_2) {
    return [cache, comp, count_1, count_2](size_t i, size_t j) {
        return cache.log_exp_series_term(comp.log_rate_const_1, i) + cache.log_exp_series_term(comp.log_rate_const_2, j) + cache.log_binomial(count_1 - i, count_2 - j);
    };
}