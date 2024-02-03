#ifndef CONVERGING_H
#define CONVERGING_H

#include "../../common/common.hpp"

#include <functional>

/*
Evaluates the log(sum over 0<=i<n, 0<=j<m: exp(terms(i, j)))
Assumes terms converge for increasing i, j
rel_accuracy should correspond approximately to proportional error on result - will be much more reliable for strictly positive terms
*/
scalar log_converging_double_sum(size_t n, size_t m, function<scalar(size_t i, size_t j)> terms, scalar rel_accuracy);

class BinLikelihoodCache : public FactorialCache {
    private:
        size_t max_count_1;
        size_t max_count_2;

        scalar detector_const_1;
        scalar detector_const_2;

        vec series_1;
        vec series_2;
    public:
        BinLikelihoodCache(size_t max_count_1, size_t max_count_2, scalar detector_const_1, scalar detector_const_2);

        function<scalar(size_t i, size_t j)> log_sum_terms(size_t count_1, size_t count_2);
};

#endif