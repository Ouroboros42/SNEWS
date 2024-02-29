#ifndef MESH_H
#define MESH_H

#include "core.hpp"
#include "full_calc.hpp"

#include <map>

/* A map of possible time differences (offsets) to their calculated likelihood */
typedef std::map<scalar, scalar> mesh;

/* Return the offsets with minimum and maximum likelihoods that are in range [start_offset, stop_offset] */
std::tuple<scalar, scalar> min_max_likelihood_in_range(mesh likelihoods, scalar start_offset, scalar stop_offset);

/* Return the minimum and maximum offset that have likelihoods higher than threshold */
std::tuple<scalar, scalar> range_over_likelihood_threshold(mesh likelihoods, scalar threshold);

std::tuple<scalar, scalar> most_likely_offset(mesh likelihoods);

/* Calculate log-likelihoods of time differences in the specified range,
calculating more points around the most likely time until a small enough range is found

Operates by repeatedly performing sweeps (iterating over offsets evenly spaced within some range). The range shrinks with each sweep.

@param signals The signals to find the lag between
@param rel_accuracy The maximum acceptable relative error on each likelihood (approximately equal to absolute error on log-likelihoods)
@param sweep_start Lowest offset in initial range of time offsets to evaluate
@param sweep_stop Highest offset in initial range of time offsets to evaluate
@param initial_sweep_steps Number of offsets to evaluate in initial sweep
@param resweep_steps Number of offsets to evaluate in subsequent sweeps of most probable sub-ranges
@param target_log_likelihood_range Difference between max and min log likelihood in current range that means sweeps can stop
@param min_window_shrink Sweeps stop if range is not reduced by at least this much each time
@param max_points Maximum total number of offsets to evaluate. Sweeps stop if this is exceeded.
*/
mesh make_recursive_likelihood_mesh(
    SignalAnalysis signals,
    scalar rel_accuracy,
    scalar sweep_start,
    scalar sweep_stop,
    size_t initial_sweep_steps,
    size_t resweep_steps,
    scalar target_log_likelihood_range,
    scalar min_window_shrink,
    size_t max_points
);

#endif