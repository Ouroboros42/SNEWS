#ifndef FACT_CACHE_H
#define FACT_CACHE_H

#include "core.hpp"

class FactorialCache {
private:
    size_t max_n;
    vec LogNumberVector;
    vec LogFactorialVector;
public:
    FactorialCache(size_t max_n);

    // largest n for which log(n!) is stored
    size_t size();

    // log(n) for 0 < n <= size
    inline scalar log(size_t n) { return LogNumberVector[n-1]; }
    
    // log(n!) for 0 <= n <= size
    inline scalar log_factorial(size_t n) { return (n == 0) ? 0 : LogFactorialVector[n-1]; }

    // Construct the series log(x^n / n!) for 0 <= n < n_terms
    vec exp_series(scalar x, size_t n_terms);

    // log((r+s) C r)
    scalar log_binomial(size_t r, size_t s);
};

#endif