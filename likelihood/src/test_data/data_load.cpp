#include "data_load.hpp"
#include "core.hpp"
#include "random.hpp"

#include <format>
#include <string>
#include <iostream>
#include <algorithm>

std::string test_data_path(Detector detector, std::string file_id) {
    return "../test-data/nlog-dump-" + detector_name(detector) + "-json-" + file_id + "-0.json";
}

TestSignal::TestSignal(Json::Value data, std::string detector_name) : TimeSeries(parse_json_array<scalar>(data["timeseries"]["times"])),
    true_time(data["truth"]["dets"][detector_name]["true_t"].asDouble())
{}

TestSignal::TestSignal(Detector detector, std::string file_id) : TestSignal(read_json_file(test_data_path(detector, file_id)), detector_name(detector)) {};

void TestSignal::filter_times() {
    std::remove_if(times.begin(), times.end(), [this](scalar time) { return start <= time && time <= stop; });
}

void TestSignal::reframe(scalar new_start, scalar new_stop) {
    bool needs_filter = new_start > start || new_stop < stop; 
    
    start = new_start;
    stop = new_stop;

    if (needs_filter) filter_times();
}

size_t TestSignal::add_background(scalar rate) {
    size_t n_background = rate * range();
    times.reserve(times.size() + n_background);

    for (size_t i = 0; i < n_background; i++) {
        times.push_back(rand_in_range(start, stop));
    }

    return n_background;
}

size_t add_background(Histogram& hist, scalar background_rate) {
    size_t n_events = background_rate * hist.range();

    for (size_t i = 0; i < n_events; i++) {
        hist.inc_bin(rand_int(hist.size()));
    }

    return n_events;
}
