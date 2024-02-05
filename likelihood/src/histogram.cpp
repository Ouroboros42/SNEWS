#include "histogram.hpp"

#include <algorithm>
#include <iterator>

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi)
: lo(lo), hi(hi), delta((hi-lo)/n_bins),
bins(n_bins), /*Initialise bins to 0*/
n_data(0) {}

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data)
: Histogram(n_bins, lo, hi) {
    add_data(data);
}

size_t Histogram::binOf(scalar datum) {
    if (datum == hi) return size() - 1; // Make last bin inclusive of boundary

    return (datum - lo)/delta; // TODO May need to be explicitly rounded if compiler implementation is weird?
}

bool Histogram::add_datum(scalar datum) {
    size_t bin_i = binOf(datum);
    bool in_range = (0 <= bin_i) && (bin_i < size());
    if (in_range) {
        ++bins.at(bin_i);
        ++n_data;
    }
    return in_range;
}

size_t Histogram::add_data(std::vector<scalar> data) {
    size_t n_in_range = 0;
    for(scalar datum : data) {
        if (add_datum(datum)) { n_in_range++; }
    }
    return n_in_range;
}

size_t Histogram::max_bin() {
    return *std::max_element(bins.begin(), bins.end());
}