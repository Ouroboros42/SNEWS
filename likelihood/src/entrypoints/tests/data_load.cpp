#define CATCH_CONFIG_MAIN

#include "test_data/data_load.hpp"

#include <catch.hpp>

#include <fstream>
#include <iostream>

TEST_CASE("Add background") {
    Histogram hist(10, -5, 5);
    std::cout << add_background(hist, 1) << std::endl;
    CHECK(hist.n_data() == 10);
}