#include "common.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

FactorialCache::FactorialCache(size_t max_n)
: max_n(max_n), LogNumberVector(max_n), LogFactorialVector(max_n) {
    double running_sum = 0;

    for (int i = 1; i <= max_n; ++i) {
        running_sum += LogNumberVector[i - 1] = std::log(i);
        LogFactorialVector[i - 1] = running_sum;
    }
}

vec FactorialCache::exp_series(scalar x, size_t n_terms) {
    scalar log_x = log(x);
    vec expansion(n_terms);

    for (size_t i = 0; i < n_terms; i++) {
        expansion[i] = (i * log_x) - log_factorial(i);
    }

    return expansion;
}

scalar FactorialCache::log_binomial(size_t r, size_t s) {
    return log_factorial(r + s) - log_factorial(r) - log_factorial(s);
}