#include "full_calc.hpp"

SignalAnalysis::SignalAnalysis(Histogram signal_1, TimeSeries signal_2, scalar background_rate_1, scalar background_rate_2) :
    signal_1(signal_1),
    signal_2(signal_2),
    background_rate_1(background_rate_1),
    background_rate_2(background_rate_2),
    detectors(background_rate_1, background_rate_2, signal_1, signal_2),
    cache()
{}

Histogram SignalAnalysis::bin_signal_2(scalar time_offset) {
    Histogram signal_2_binned = Histogram(signal_1.size(), signal_1.lo() + time_offset, signal_1.hi() + time_offset);
    signal_2_binned.add_data(signal_2.times);
    return signal_2_binned;
}


scalar SignalAnalysis::lag_log_likelihood(scalar time_difference_2_after_1, scalar rel_precision, bool reevaluate_sensitivity) {
    Histogram signal_2_binned = bin_signal_2(time_difference_2_after_1);

    DetectorRelation trial_detectors = reevaluate_sensitivity ?
        DetectorRelation(background_rate_1, background_rate_2, signal_1, signal_2_binned) : 
        detectors;

    return log_likelihood(cache, trial_detectors, signal_1, signal_2_binned, rel_precision);
}
