#ifndef CONVERGING_H
#define CONVERGING_H

#include "fast_sum/lazy.hpp"

#include <functional>

typedef std::function<size_t(size_t index_1)> index_getter;

/*
Add sequence of terms defined by log_terms(i) for 0 <= i < n
Assumes strictly decreasing terms
Rejects terms of value (relative to total) below precision
*/   
scalar converging_sum_exp(LazyArray const& log_terms, scalar total, scalar log_rescale, scalar term_rel_precision);

scalar log_sum_exp_peaked_2D(Lazy2DArray& terms, size_t lead_i, index_getter get_lead_j, scalar rel_precision);

#endif