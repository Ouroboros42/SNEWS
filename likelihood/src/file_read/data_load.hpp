#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include "core.hpp"

#include <json.hpp>

#include <vector>
#include <string>

enum Detector {
    IceCube,
    SuperK,
    SNOPlus,
};

std::string detector_name(Detector detector);
scalar background_rates_ms(Detector detector);

std::string data_path(Detector detector);

Json::Value get_data(std::string path);

struct DetectorSignal {
    std::vector<scalar> time_series;

    scalar start_time;
    scalar end_time;

    scalar true_time;

    scalar background_rate_ms;

    DetectorSignal(Json::Value data, std::string detector_name, scalar background_rate);

    DetectorSignal(Detector detector);
};

#endif