#include "common.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

FactorialCache::FactorialCache(size_t max_n) : max_n(max_n) {
    LogNumberVector = vec(max_n);
    LogFactorialVector = vec(max_n);

    buildVectors();
}

void FactorialCache::buildVectors() {
    double running_sum = 0;

    for (int i = 1; i <= max_n; ++i) {
        running_sum += LogNumberVector[i - 1] = std::log(i);
        LogFactorialVector[i - 1] = running_sum;
    }
}

vec FactorialCache::exp_series(scalar x) {
    scalar log_x = log(x);
    vec expansion(max_n);

    for (size_t i = 0; i < max_n; i++) {
        expansion[i] = (i * log_x) - log_factorial(i);
    }

    return expansion;
}

scalar FactorialCache::log_binomial(size_t r, size_t s) {
    return log_factorial(r + s) - log_factorial(r) - log_factorial(s);
}