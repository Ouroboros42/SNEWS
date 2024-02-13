#include "histogram.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

// default constructor
Histogram::Histogram(size_t n_bins, scalar lo, scalar hi)
: lo(lo), hi(hi), delta((hi-lo)/n_bins),
bins(n_bins), /*Initialise bins to 0*/
n_data(0) {}

// constructor with data
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
        inc_bin(bin_i);
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

scalar Histogram::range() {
    return hi - lo;
}

scalar Histogram::mean_rate() {
    return n_data / range();
}

std::string Histogram::display() {
    std::stringstream out;
    out << "[";
    for (size_t i = 0; i < bins.size(); i++) {
        if (i != 0) {
            out << ", ";
        }
        out << bins[i];
    }
    out << "]";
    return out.str();
}