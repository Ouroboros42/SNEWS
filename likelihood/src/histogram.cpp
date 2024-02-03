#include "histogram.hpp"

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi)
: lo(lo), hi(hi), delta((hi-lo)/n_bins), bins(n_bins) /*Initialise bins to 0*/ {}

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data)
: Histogram(n_bins, lo, hi) {
    addData(data);
}

size_t Histogram::binOf(scalar datum) {
    if (datum == hi) return n_bins - 1; // Make last bin inclusive of boundary

    return (datum - lo)/delta; // TODO May need to be explicitly rounded if compiler implementation is weird?
}

void Histogram::addDatum(scalar datum) {
    ++bins[binOf(datum)];
}

void Histogram::addData(std::vector<scalar> data) {
    for(scalar datum : data) {
        addDatum(datum);
    }
}