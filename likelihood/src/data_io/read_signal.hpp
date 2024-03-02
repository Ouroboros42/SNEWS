#ifndef READ_SIGNAL_H
#define READ_SIGNAL_H

#include "core.hpp"
#include "data_io/histogram.hpp"

#include <json.hpp>

Json::Value read_json_file(path filepath);

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

    inline scalar range() { return stop - start; }

    inline size_t n_events() { return times.size(); }

    inline scalar mean_rate() { return n_events() / range(); }
    
    Histogram to_hist(size_t n_bins, scalar from, scalar to);
    
    Histogram to_hist(size_t n_bins);

    Histogram to_hist(scalar bin_width);

    void rezero_times(scalar zero_time);
};

#endif