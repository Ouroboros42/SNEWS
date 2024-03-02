#ifndef WRITE_OUT_H
#define WRITE_OUT_H

#include "core.hpp"
#include "mesh.hpp"
#include "data_io/histogram.hpp"
#include "detector_info/detectors.hpp"

#include <json.hpp>

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

path output_filename(Detector detector_1, Detector detector_2, std::string test_inst);

// Store a calculated likelihood distribution and associated information in specified file
void save_likelihoods(
    path output_path,
    SignalAnalysis signals,
    mesh likelihood_mesh,
    scalar true_time_difference
);

#endif