#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "caching/factorials.hpp"

#include "fast_sum/converging.hpp"

#include <cmath>

class ExpSeries : public LazyArray {
    FactorialCache& cache;
    size_t n;
    scalar log_x;
public:
    ExpSeries(FactorialCache& cache, size_t n, scalar x) : cache(cache), n(n), log_x(std::log(x)) {
        cache.build_upto(n);
    }

    size_t size() const { return n; }

    scalar get(size_t i) const { return cache.log_exp_series_term(log_x, i); }
};

TEST_CASE("Convering single sum") {
    size_t n = 1000;
    scalar x = 0.1;
    scalar eps = 0.0000001;

    FactorialCache cache;
    ExpSeries series(cache, n, x);

    CHECK_THAT(converging_sum_exp(series, 0, 0, eps), Catch::Matchers::WithinRel(std::exp(x), 0.03 + eps));
}