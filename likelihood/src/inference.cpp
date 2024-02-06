#include "inference.hpp"

scalar expected_neutrino_events(size_t n_events, scalar period, scalar background_rate) {
    return n_events - (background_rate * period);
}

scalar sensitivity_ratio(size_t n_top, size_t n_bottom, scalar period, scalar background_top, scalar background_bottom) {
    return expected_neutrino_events(n_top, period, background_top) / expected_neutrino_events(n_bottom, period, background_bottom);
}