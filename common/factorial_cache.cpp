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