#include "detector_info/detectors.hpp"
#include "test_data/data_load.hpp"

#include <CLI11.hpp>

CLI::Transformer detector_validator(std::map<std::string, Detector>{
    { "SNOP", Detector::SNOPlus },
    { "SK", Detector::SuperK },
    { "IC", Detector::IceCube }
});

int main(int argc, char** argv) {
    try {
        CLI::App app { "Evaluate likelihood distribution for test data" };
        argv = app.ensure_utf8(argv);

        Detector detector_1, detector_2;
        app.add_option("Detector-1,--d1", detector_1, "Abbreviated name of detector of first signal")->transform(detector_validator);
        app.add_option("Detector-2,--d2", detector_2, "Abbreviated name of detector of second signal")->transform(detector_validator);

        std::string test_inst;
        app.add_option("Test-Instance,--inst", test_inst, "3 digit id number of test data to use")
            ->check(CLI::TypeValidator<unsigned int>() & CLI::Range(0,999)); // Checks value is 3 digits but does not convert type 

        path signal_directory = (path) ".." / "test-data";
        app.add_option("-s,--sig-dir,--signal-dir", signal_directory, "Directory to find signal files");

        CLI11_PARSE(app, argc, argv);

        path source_1 = signal_directory / test_data_file(detector_1, test_inst);
        path source_2 = signal_directory / test_data_file(detector_2, test_inst);

        auto [test_signals, true_lag] = complete_test_case(
            detector_1, detector_2, source_1, source_2,
            1, 20, -0.2, 0.2, true, 2e-3
        );

        std::printf("True lag = %.10f\n", true_lag);
    } catch(const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}