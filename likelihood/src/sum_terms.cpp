#include "sum_terms.hpp"

#include <cassert>

BinSumTerms::BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2) :
    count_1(count_1), count_2(count_2), fcache(fcache), detectors(detectors)
{
    fcache.build_upto(count_1 + count_2);
}

size_t BinSumTerms::size_x() const { return count_1 + 1; }

size_t BinSumTerms::size_y() const { return count_2 + 1; }

scalar BinSumTerms::get(size_t i, size_t j) const {
    return fcache.log_exp_series_term(detectors.log_rate_const_1, i) +
        fcache.log_exp_series_term(detectors.log_rate_const_2, j) +
        fcache.log_binomial(count_1 - i, count_2 - j);
}

scalar bin_log_likelihood_unscaled(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision) {
    BinSumTerms terms(fcache, detectors, count_1, count_2);

    return log_sum_exp_peaked_2D(
        terms,
        detectors.lead_index_1(count_1, count_2),
        [detectors, count_1, count_2] (size_t row_i) { return detectors.lead_index_2(count_1, count_2, row_i); },
        rel_precision
    );
}

scalar log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, Histogram signal_1, Histogram signal_2, scalar rel_precision) {
    assert(signal_1.size() == signal_2.size());
    size_t n_bins = signal_1.size();

    scalar likelihood = detectors.log_likelihood_prefactor(signal_1.n_data(), signal_2.n_data());

    for (size_t i_bin = 0; i_bin < n_bins; i_bin++) {
        likelihood += bin_log_likelihood_unscaled(fcache, detectors, signal_1[i_bin], signal_2[i_bin], rel_precision);
    }

    return likelihood;
}