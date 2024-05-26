#include "detector_info/detectors.hpp"
#include "test_data/data_load.hpp"
#include "mesh.hpp"
#include "data_io/write_output.hpp"

#include <CLI11.hpp>

#include <chrono>

CLI::Transformer detector_validator(std::map<std::string, Detector>{
    { "SNOP", Detector::SNOPlus },
    { "SK", Detector::SuperK },
    { "IC", Detector::IceCube },
    { "DS-20K", Detector::DarkSide }
});

int main(int argc, char** argv) {
    try {
        CLI::App app { "Evaluate time lag likelihood distribution for given test data" };
        argv = app.ensure_utf8(argv);

        Detector detector_1, detector_2;
        app.add_option("Detector-1,--d1", detector_1, "Abbreviated name of detector of first signal")->transform(detector_validator);
        app.add_option("Detector-2,--d2", detector_2, "Abbreviated name of detector of second signal")->transform(detector_validator);

        std::string test_inst;
        app.add_option("Test-Instance,--inst", test_inst, "3 digit id number of test data to use")
            ->check(CLI::TypeValidator<unsigned int>() & CLI::Range(0,999)); // Checks value is 3 digits but does not convert type 

        path signal_directory = (path) ".." / "test-data";
        app.add_option("-s,--sig-dir,--signal-dir", signal_directory, "Directory to find signal files");

        path output_directory = "output";
        app.add_option("-o,--out-dir,--output-dir", output_directory, "Directory to put output files");

        scalar front_buffer = 1;
        app.add_option("--front-buffer", front_buffer, "Time before true events to include in sampled window");
        
        scalar window_size = 20;
        app.add_option("-w,--window", window_size, "Length of time window to sample");

        std::pair<scalar, scalar> sweep_range = { -.1, .1 };
        app.add_option("-r,--sweep-range", sweep_range, "Widest range of time offsets to sample");

        bool vary_background = true;
        app.add_flag("--fix-background", vary_background, "Use a fixed total number of background events (without this flag, will be Poisson distributed)");

        scalar bin_width = 2e-3;
        app.add_option("-b,--bin-width", bin_width, "Width of histogram bins for binning neutrino event times");

        scalar rel_accuracy = 1e-3;
        app.add_option("-a,--accuracy", rel_accuracy, "Maximum acceptable relative error on each likelihood");

        scalar target_log_likelihood_range = 0.5;
        app.add_option("-t,--target-lrange", target_log_likelihood_range, "Target range of log-likelihoods around maxima, will stop sweeping if sweep is within this range.");

        size_t initial_sweep_steps = 100;
        app.add_option("--init-sweep-n", initial_sweep_steps, "Number of likelihoods to calculate in initial sweep of entire time range");

        size_t resweep_steps = 50;
        app.add_option("--resweep-n", resweep_steps, "Number of likelihoods to calculate in secondary sweeps of refined time ranges around maximum likelihood");

        scalar min_sweep_shrink = 1e-4;
        app.add_option("--min-sweep-shrink", min_sweep_shrink, "Minimum reduction in sweep range between sweeps, will stop if not achieved.");

        size_t max_points = 400;
        app.add_option("-n,--max-points", max_points, "Maximum total number of likelihoods to calculate, will stop once if exceeded.");

        CLI11_PARSE(app, argc, argv);

        path source_1 = signal_directory / test_data_file(detector_1, test_inst);
        path source_2 = signal_directory / test_data_file(detector_2, test_inst);

        path output_file = output_directory / output_filename(detector_1, detector_2, test_inst);

        auto FULL_START = mark_time();

        auto [test_signals, true_lag] = complete_test_case(
            detector_1, detector_2,
            source_1, source_2,
            front_buffer, window_size,
            sweep_range.first, sweep_range.second,
            vary_background,
            bin_width
        );

        std::printf("Detector Sensitivity Ratio = %.10f\n", test_signals.detectors.rate_const_ratio_2_to_1);

        auto SWEEP_START = mark_time();

        std::printf("Data loaded and prepared in %us\n", TIME_BETWEEN(sec, FULL_START, SWEEP_START));

        mesh likelihoods = make_recursive_likelihood_mesh(
            test_signals,
            rel_accuracy, 
            sweep_range.first, sweep_range.second,
            initial_sweep_steps, resweep_steps,
            target_log_likelihood_range,
            min_sweep_shrink,
            max_points
        );
        
        auto SWEEP_END = mark_time();

        std::printf("Likelihoods calculated in %us\n", TIME_BETWEEN(sec, SWEEP_START, SWEEP_END));

        save_likelihoods(output_file, test_signals, likelihoods, true_lag);

        auto FULL_END = mark_time();

        std::printf("Total time elapsed: %us\n", TIME_BETWEEN(sec, FULL_START, FULL_END));
    } catch(const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}