#ifndef FULL_CALC_H
#define FULL_CALC_H

#include "data_io/histogram.hpp"
#include "data_io/read_signal.hpp"
#include "detector_info/relation.hpp"
#include "fast_sum/sum_terms.hpp"

/* Aggregate of all reuseable information when calculating likelihoods for multiple time differences */
struct SignalAnalysis {
    /* Larger signal, pre-binned for speed */
    Histogram signal_1;
    /* Smaller signal, will be re-binned for each time difference */
    TimeSeries signal_2;

    /* Detector information, constant across trials */
    DetectorRelation detectors;

    /* Log factorials to be reused */
    FactorialCache cache;

    /* Build new instance for given signals with known background rates
    Infer detector ratios from full signals
    Use a fresh factorial cache */
    SignalAnalysis(Histogram signal_1, TimeSeries signal_2, scalar background_rate_1, scalar background_rate_2);

    /* Put signal 2 into a histogram of the same size as signal 1, but over a shifted time window.
    Positive time_offset means binning events as if they occured that much earlier */
    Histogram bin_signal_2(scalar time_offset);

    /* Calculate log likelihood of signal 2 arriving some time difference after signal 1
    rel_precision is maximum acceptable (proportional) error in calculated likelihood */
    scalar lag_log_likelihood(scalar time_difference_2_after_1, scalar rel_precision) {
        Histogram signal_2_binned = bin_signal_2(time_difference_2_after_1);
        return log_likelihood(cache, detectors, signal_1, signal_2_binned, rel_precision);
    }
};

#endif