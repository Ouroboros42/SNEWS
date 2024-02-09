#define CATCH_CONFIG_MAIN

#include "test_data/data_load.hpp"

#include <catch.hpp>

#include <fstream>
#include <iostream>

TEST_CASE("Simple read") {
    DetectorSignal skdata(Detector::SuperK);

    REQUIRE(skdata.true_time == 1635744156.3231735);
    REQUIRE(skdata.time_series.size() > 1000);
    REQUIRE(skdata.background_rate_ms == 0.0001);
}

TEST_CASE("Add background") {
    Histogram hist(10, -5, 5);
    std::cout << add_background(hist, 1) << std::endl;
    REQUIRE(hist.get_n_data() == 10);
}