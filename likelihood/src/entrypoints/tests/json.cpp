#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>

TEST_CASE("Simple read") {
    Json::Value SKData;
    std::ifstream SKFile("../temp-data/nlog-dump-SK-json-121-0.json", std::ifstream::binary);
    SKFile >> SKData;

    REQUIRE_NOTHROW(SKData["truth"]["true_sn_ra"].asDouble());
}