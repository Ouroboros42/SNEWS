#include "histogram.hpp"

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi)
: lo(lo), hi(hi), delta((hi-lo)/n_bins), bins(n_bins) /*Initialise bins to 0*/ {}

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data)
: Histogram(n_bins, lo, hi) {
    addData(data);
}

size_t Histogram::binOf(scalar datum) {
    if (datum == hi) return getNBins() - 1; // Make last bin inclusive of boundary

    return (datum - lo)/delta; // TODO May need to be explicitly rounded if compiler implementation is weird?
}

bool Histogram::addDatum(scalar datum) {
    size_t bin_i = binOf(datum);
    bool in_range = (0 <= bin_i) && (bin_i < getNBins());
    if (in_range) { ++bins.at(bin_i); }
    return in_range;
}

size_t Histogram::addData(std::vector<scalar> data) {
    size_t n_in_range = 0;
    for(scalar datum : data) {
        if (addDatum(datum)) { n_in_range++; }
    }
    return n_in_range;
}