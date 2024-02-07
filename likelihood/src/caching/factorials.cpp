#include "factorials.hpp"

#include <cmath>

FactorialCache::FactorialCache()
: max_n(1), LogNumberVector { 0 }, LogFactorialVector { 0, 0 } {}

FactorialCache::FactorialCache(size_t max_n) : FactorialCache() {
    build_upto(max_n);
}

void FactorialCache::build_upto(size_t new_max_n) {
    if (new_max_n <= max_n) return;

    scalar running_sum = log_factorial(max_n);

    LogNumberVector.resize(new_max_n);
    LogFactorialVector.resize(new_max_n + 1);

    for (size_t i = max_n + 1; i <= new_max_n; i++) {
        running_sum += LogNumberVector[i - 1] = std::log(i);
        LogFactorialVector[i] = running_sum;
    }
}

scalar FactorialCache::log_exp_series_term(scalar log_x, size_t index) {
    return (index * log_x) - log_factorial(index);
}

vec FactorialCache::exp_series(scalar x, size_t n_terms) {
    scalar log_x = log(x);
    vec expansion(n_terms);

    for (size_t i = 0; i < n_terms; i++) {
        expansion[i] = log_exp_series_term(log_x, i);
    }

    return expansion;
}

scalar FactorialCache::log_binomial(size_t r, size_t s) {
    return log_factorial(r + s) - log_factorial(r) - log_factorial(s);
}