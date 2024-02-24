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



