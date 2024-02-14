#include "detector_params.hpp"

#include <cmath>

scalar quadratic_low_root(scalar b, scalar c) {
    scalar half_b = b / 2;

    return - half_b - std::sqrt(half_b * half_b - c);
}

scalar quadratic_low_root(scalar a, scalar b, scalar c) {
    return quadratic_low_root(b / a, c / a);
}


DetectorParams::DetectorParams(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_ratio_2_to_1)
: DetectorParams (
    background_rate_1, background_rate_2,
    1 / (1 + sensitivity_ratio_2_to_1),
    1 / (1 + 1 / sensitivity_ratio_2_to_1)
) {}

DetectorParams::DetectorParams(scalar background_rate_1, scalar background_rate_2, scalar sensitivity_1, scalar sensitivity_2) : 
    log_sensitivity_1(std::log(sensitivity_1)),
    log_sensitivity_2(std::log(sensitivity_2)),
    rate_const_1(background_rate_1 / sensitivity_1),
    rate_const_2(background_rate_2 / sensitivity_2),
    log_rate_const_1(std::log(rate_const_1)),
    log_rate_const_2(std::log(rate_const_2)),
    rate_const_ratio_2_to_1(rate_const_2 / rate_const_1)
{}

DetectorParams::DetectorParams(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2)
: DetectorParams(
    background_rate_1, background_rate_2,
    (events_2.mean_rate() - background_rate_2) / (events_1.mean_rate() - background_rate_1)
) {}

scalar DetectorParams::log_likelihood_prefactor(size_t total_events_1, size_t total_events_2) {
    return log_sensitivity_1 * total_events_1 + log_sensitivity_2 * total_events_2;
}

std::function<scalar(size_t i, size_t j)> log_sum_terms(FactorialCache cache, DetectorParams comp, size_t count_1, size_t count_2) {
    return [cache, comp, count_1, count_2](size_t i, size_t j) {
        return cache.log_exp_series_term(comp.log_rate_const_1, i) + cache.log_exp_series_term(comp.log_rate_const_2, j) + cache.log_binomial(count_1 - i, count_2 - j);
    };
}

size_t normalise_index(size_t max_index, scalar index_low_bound) {
    size_t index = std::ceil(index_low_bound);

    return std::min(std::max((size_t) 0, index), max_index);
}

size_t DetectorParams::lead_index_1(size_t count_1, size_t count_2) {
    if (log_rate_const_1 < 0) { return 0; }

    return normalise_index(count_1,
        quadratic_low_root(
            1 - rate_const_ratio_2_to_1,
            rate_const_2 - rate_const_1 - 2 * rate_const_ratio_2_to_1 - count_1 - count_2,
            rate_const_2 - rate_const_ratio_2_to_1 - count_2 + (rate_const_1 - 1) * count_1 - 1
        )
    );
}

size_t DetectorParams::lead_index_2(size_t count_1, size_t count_2, size_t index_1) const {
    if (log_rate_const_2 < 0) { return 0; }

    return normalise_index(count_2,
        quadratic_low_root(
            1 - (scalar) count_1 - count_2 + index_1 - rate_const_2,
            (rate_const_2 - 1) * count_2 + index_1 - count_1
        )
    );
}