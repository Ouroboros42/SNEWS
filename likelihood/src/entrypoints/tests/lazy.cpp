#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "fast_sum/lazy.hpp"

#include<iostream>

class PowerGrid : public Lazy2DArray {
private:
    size_t n;
    size_t m;
public:
    PowerGrid(size_t n, size_t m) : n(n), m(m) {}

    size_t size_x() const { return n; }

    size_t size_y() const { return m; }

    scalar get(size_t i_x, size_t i_y) const {
        return std::pow(i_x, i_y);
    }
};

TEST_CASE("Row access test") {
    PowerGrid grid(10, 20);
    LazySubArray row2(grid, 2, 7, false);

    CHECK(row2.get(0) == std::pow(2, 7));
    CHECK(row2.get(1) == std::pow(2, 6));
    CHECK(row2.get(2) == std::pow(2, 5));
    CHECK(row2.get(3) == std::pow(2, 4));
    CHECK(row2.get(4) == std::pow(2, 3));
    CHECK(row2.size() == 8);
}