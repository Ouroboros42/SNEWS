#include "read_signal.hpp"
#include "util/array_util.hpp"

#include <fstream>
#include <cmath>

Json::Value read_json_file(std::string path) {
    Json::Value data;
    std::ifstream file(path, std::ifstream::binary);
    file >> data;
    return data;
}

TimeSeries::TimeSeries(Json::Value timeseries) :
    times(parse_json_array<scalar>(timeseries["times"])),
    start(timeseries["start"].asDouble()),
    stop(timeseries["stop"].asDouble())
{}

TimeSeries::TimeSeries(vec times) :
    times(times),
    start(min(times)),
    stop(max(times))
{}

Histogram TimeSeries::to_hist(size_t n_bins, scalar from, scalar to) {
    return Histogram(n_bins, from, to, times);
}

Histogram TimeSeries::to_hist(size_t n_bins) {
    return to_hist(n_bins, start, stop);
}

Histogram TimeSeries::to_hist(scalar bin_width) {
    return to_hist((size_t) std::abs(std::ceil(range() / bin_width)));
}

void TimeSeries::rezero_times(scalar zero_time) {
    for (size_t i = 0; i < times.size(); i++) {
        times[i] -= zero_time;
    }

    start -= zero_time;
    stop -= zero_time;
}