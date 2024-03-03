#include "write_output.hpp"
#include "util/array_util.hpp"
#include "data_io/timestamp.hpp"

#include <string>
#include <fstream>


path output_filename(Detector detector_1, Detector detector_2, std::string test_inst) {
    return "ldist_" + detector_name(detector_1) + "-vs-" + detector_name(detector_2) + "_src=" + test_inst + "_t=" + get_timestamp() + ".json";
}

void save_likelihoods(
    path output_path,
    SignalAnalysis signals,
    mesh likelihood_mesh,
    scalar true_time_difference
) {
    std::cout << "Saving output to: " << output_path << std::endl;

    Json::Value outputs;

    auto [offsets, likelihoods] = unzip(likelihood_mesh);

    Histogram& signal_1 = signals.signal_1;
    TimeSeries& signal_2 = signals.signal_2;

    // Examples of the signal 2 binnings, for debugging
    std::vector<Histogram> hist_2_samples {
        signals.bin_signal_2(signal_2.start - signal_1.lo()),
        signals.bin_signal_2(0),
        signals.bin_signal_2(signal_2.stop - signal_1.hi())
    };

    // Add additional information by assigning to unused keys, no extra setup needed
    outputs["Time-Difference"] = json_array(offsets);
    outputs["Likelihood"] = json_array(likelihoods);
    outputs["Binned"]["Signal-1"] = json_array(signal_1);
    outputs["Binned"]["Signals-2"] = json_2D_array(hist_2_samples);
    outputs["Binned"]["Window-Width"] = signal_1.range();
    outputs["True-Time-Difference"] = true_time_difference;

    Json::StreamWriterBuilder builder;
    std::string output_str = Json::writeString(builder, outputs);

    std::filesystem::create_directories(output_path.parent_path());

    std::ofstream output_file;
    output_file.open(output_path);
    output_file << output_str;
    output_file.close();
}
