#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "core.hpp"
#include "data_io/histogram.hpp"

#include <string>

template <typename T>
Json::Value json_array(std::vector<T> arr) {
    Json::Value arr_out(Json::arrayValue);
    for (T elem : arr) {
        arr_out.append(elem);
    }
    return arr_out;
}

template <typename T1>
Json::Value json_2D_array(std::vector<T1> arr2D) {
    Json::Value arr2D_out(Json::arrayValue);
    for (T1 arr1D : arr2D) {
        arr2D_out.append(json_array(arr1D));
    }
    return arr2D_out;
}

// Store a calculated likelihood distribution and associated information in specified filename
void save_likelihoods(
    std::string output_filename,
    vec time_diffs, vec likelihoods,
    Histogram dist1, std::vector<Histogram> dist2s,
    scalar window_width, scalar true_time_difference
);

#endif