#ifndef SUM_TERMS_H
#define SUM_TERMS_H

#include "fast_sum/lazy.hpp"
#include "caching/factorials.hpp"
#include "detector_info/relation.hpp"

class BinSumTerms : public Lazy2DArray {
public:
    size_t count_1;
    size_t count_2;

    const FactorialCache& fcache;
    const DetectorRelation& detectors;

    BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2);

    size_t size_x() const;
    size_t size_y() const;

    scalar get(size_t i_x, size_t i_y) const;
};

scalar bin_log_likelihood_unscaled(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision);

scalar log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, Histogram signal_1, Histogram signal_2, scalar rel_precision);

#endif