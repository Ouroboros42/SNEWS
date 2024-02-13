#include "data_load.hpp"
#include "core.hpp"
#include "random.hpp"

#include <format>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

std::string detector_name(Detector detector) {
    switch (detector) {
        case Detector::IceCube:
            return "IC";
        case Detector::SNOPlus:
            return "SNOP";
        case Detector::SuperK:
            return "SK";
        default:
            throw std::invalid_argument("Undefined detector");
    }
}

scalar background_rates_ms(Detector detector) {  // TODO Confirm accuracy
    switch (detector) { 
        case Detector::IceCube:
            return 1500;
        case Detector::SNOPlus:
            return 0.0003;
        case Detector::SuperK:
            return 0.0001;
        default:
            throw std::invalid_argument("Undefined detector");
    }
}

std::string data_path(Detector detector) {
    return "../temp-data/nlog-dump-" + detector_name(detector) + "-json-121-0.json";
}

Json::Value get_data(std::string path) {
    Json::Value data;
    std::ifstream file(path, std::ifstream::binary);
    file >> data;
    return data;
}

// Map nulls to default
double double_or_default(Json::Value value, double for_null) {
    if (value.isNull()) {
        return for_null;
    } else {
        return value.asDouble();
    }
}

std::vector<double> read_double_array(Json::Value array) {
    std::vector<double> out_vec;
    out_vec.reserve(array.size());
    for (Json::Value element : array) {
        out_vec.push_back(element.asDouble());
    }
    return out_vec;
}

scalar min_elem(vec values) {
    return *std::min_element(std::begin(values), std::end(values));
}

scalar max_elem(vec values) {
    return *std::max_element(std::begin(values), std::end(values));
}

DetectorSignal::DetectorSignal(Json::Value data, std::string detector_name, scalar background_rate) :
time_series(read_double_array(data["timeseries"]["times"])),
start_time(double_or_default(data["timeseries"]["start"], min_elem(time_series))),
end_time(double_or_default(data["timeseries"]["stop"], max_elem(time_series))),
true_time(data["truth"]["dets"][detector_name]["true_t"].asDouble()),
background_rate_ms(background_rate)
{}

DetectorSignal::DetectorSignal(Detector detector) : DetectorSignal(get_data(data_path(detector)), detector_name(detector), background_rates_ms(detector)) {};

size_t add_background(Histogram& hist, scalar background_rate) {
    size_t n_events = background_rate * hist.range();

    for (size_t i = 0; i < n_events; i++) {
        hist.inc_bin(rand_int(hist.size()));
    }

    return n_events;
}

size_t DetectorSignal::add_background(scalar rate, scalar start, scalar end) {
    size_t n_background = rate * (end - start);
    time_series.reserve(time_series.size() + n_background);

    for (size_t i = 0; i < n_background; i++) {
        time_series.push_back(rand_in_range(start, end));
    }

    return n_background;
}