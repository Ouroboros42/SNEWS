#include "core.hpp"
#include "histogram.hpp"

#include <json.hpp>

Json::Value read_json_file(std::string path);

template <typename T>
std::vector<T> parse_json_array(Json::Value& array) {
    std::vector<T> out_vec;
    out_vec.reserve(array.size());
    for (Json::Value element : array) {
        out_vec.push_back(element.as<T>());
    }
    return out_vec;
}

/* Structure to represent a detector signal, over specified time range */
struct TimeSeries {
    vec times;
    
    scalar start;
    scalar stop;

    // Load json value specifying times, start and stop
    TimeSeries(Json::Value timeseries);
    
    // Directly build from timeseries, taking start and stop to be extrema of series
    TimeSeries(vec times);

    inline scalar range() { return stop - start; };
    
    Histogram to_hist(size_t n_bins, scalar from, scalar to);
    
    Histogram to_hist(size_t n_bins);
};