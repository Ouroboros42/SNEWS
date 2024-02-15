#include "write_output.hpp"

#include <json.hpp>

#include <string>
#include <fstream>

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

void save_likelihoods(std::string output_filename, vec time_diffs, vec likelihoods, Histogram dist1, std::vector<Histogram> dist2s) {
    Json::Value outputs;
    
    outputs["Time-Difference"] = json_array(time_diffs);
    outputs["Likelihood"] = json_array(likelihoods);
    outputs["Histogram1"] = json_array(dist1);
    outputs["Histograms2"] = json_2D_array(dist2s);

    Json::StreamWriterBuilder builder;
    std::string output_str = Json::writeString(builder, outputs);

    std::ofstream output_file;
    output_file.open(output_filename);
    output_file << output_str;
    output_file.close();
}
