#ifndef TERM_CACHE_H
#define TERM_CACHE_H

#include "factorials.hpp"

#include <functional>

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

        std::function<scalar(size_t i, size_t j)> log_sum_terms(size_t count_1, size_t count_2);
};

#endif
