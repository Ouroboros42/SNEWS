#include "relation.hpp"
#include "converging.hpp"

#include <cmath>

DetectorRelation::DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1)
: DetectorRelation (
    bin_background_rate_1, bin_background_rate_2,
    1 / (1 + sensitivity_ratio_2_to_1),
    1 / (1 + 1 / sensitivity_ratio_2_to_1)
) {}

DetectorRelation::DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_1, scalar sensitivity_2) : 
    log_sensitivity_1(std::log(sensitivity_1)),
    log_sensitivity_2(std::log(sensitivity_2)),
    rate_const_1(bin_background_rate_1 / sensitivity_1),
    rate_const_2(bin_background_rate_2 / sensitivity_2),
    log_rate_const_1(std::log(rate_const_1)),
    log_rate_const_2(std::log(rate_const_2)),
    rate_const_ratio_2_to_1(rate_const_2 / rate_const_1)
{}

DetectorRelation::DetectorRelation(scalar background_rate_1, scalar background_rate_2, Histogram events_1, Histogram events_2)
: DetectorRelation(
    background_rate_1 * events_1.delta(), background_rate_2 * events_2.delta(),
    (events_2.mean_rate() - background_rate_2) / (events_1.mean_rate() - background_rate_1)
) {}

scalar DetectorRelation::log_likelihood_prefactor(size_t total_events_1, size_t total_events_2) {
    return log_sensitivity_1 * total_events_1 + log_sensitivity_2 * total_events_2;
}

scalar quadratic_low_root(scalar b, scalar c) {
    scalar half_b = b / 2;
    scalar discriminant = half_b * half_b - c;
    scalar result = - half_b - std::sqrt(discriminant);

    if (result == 0) { // Limit of numerical accuracy for quadratic, can be approximated linear
        return - c / b;
    }

    return result;
}

scalar quadratic_low_root(scalar a, scalar b, scalar c) {
    if (a == 0) return - c / b;
    scalar b_scaled = b / a, c_scaled = c / a;
    return quadratic_low_root(b_scaled, c_scaled);
}

// Convert a real to a the next largest postive integer <= max_index, >= 0
size_t normalise_index(scalar index_low_est, size_t max_index) {
    if (index_low_est < 0) return 0;
    if (index_low_est > max_index) return max_index;

    return std::ceil(index_low_est);
}

size_t DetectorRelation::lead_index_1(size_t count_1, size_t count_2) {
    if (log_rate_const_1 < 0) { return 0; }

    scalar estimate = quadratic_low_root(
        1 - rate_const_ratio_2_to_1,
        rate_const_2 - rate_const_1 - 2 * rate_const_ratio_2_to_1 - count_1 - count_2,
        rate_const_2 - rate_const_ratio_2_to_1 - count_2 + (rate_const_1 - 1) * count_1 - 1
    );

    return normalise_index(estimate, count_1);
}

size_t DetectorRelation::lead_index_2(size_t count_1, size_t count_2, size_t index_1) const {
    if (log_rate_const_2 < 0) { return 0; }

    scalar estimate = quadratic_low_root(
        1 - (scalar) count_1 - count_2 + index_1 - rate_const_2,
        (rate_const_2 - 1) * count_2 + index_1 - count_1
    );

    return normalise_index(estimate, count_2);
}