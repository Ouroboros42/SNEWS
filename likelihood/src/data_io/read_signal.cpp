#include "read_signal.hpp"

#include <fstream>

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
