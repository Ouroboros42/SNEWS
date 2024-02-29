#include "write_output.hpp"
#include "util/array_util.hpp"

#include <string>
#include <fstream>

void save_likelihoods(
    std::string output_filename,
    mesh likelihood_mesh,
    Histogram dist1, std::vector<Histogram> dist2s,
    scalar window_width, scalar true_time_difference
) {
    Json::Value outputs;

    auto [offsets, likelihoods] = unzip(likelihood_mesh);

    // Add additional information by assigning to unused keys, no extra setup needed
    outputs["Time-Difference"] = json_array(offsets);
    outputs["Likelihood"] = json_array(likelihoods);
    outputs["Binned"]["Signal-1"] = json_array(dist1);
    outputs["Binned"]["Signals-2"] = json_2D_array(dist2s);
    outputs["Binned"]["Window-Width"] = window_width;
    outputs["True-Time-Difference"] = true_time_difference;

    Json::StreamWriterBuilder builder;
    std::string output_str = Json::writeString(builder, outputs);

    std::ofstream output_file;
    output_file.open(output_filename);
    output_file << output_str;
    output_file.close();
}
