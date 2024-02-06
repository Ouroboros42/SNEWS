#define CATCH_CONFIG_MAIN

#include "file_read/data_load.hpp"

#include <catch.hpp>

#include <fstream>
#include <iostream>

TEST_CASE("Simple read") {
    DetectorSignal skdata(Detector::SuperK);

    REQUIRE(skdata.true_time == 1635744156.3231735);
    REQUIRE(skdata.time_series.size() > 1000);
    REQUIRE(skdata.background_rate_ms == 0.0003);
}