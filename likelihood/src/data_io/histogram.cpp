#include "data_io/histogram.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <numeric>

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi)
: vector(n_bins, 0),
    _lo(lo), _hi(hi),
    _delta((hi-lo)/n_bins)
{}

Histogram::Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data)
: Histogram(n_bins, lo, hi) {
    add_data(data);
}

Histogram::Histogram(std::vector<size_t> bins, scalar lo, scalar hi)
: vector(bins),
    _lo(lo), _hi(hi),
    _delta((hi-lo)/bins.size())
{}

Histogram Histogram::empty(Histogram like) {
    return Histogram(like.size(), like.lo(), like.hi());
}

size_t Histogram::binOf(scalar datum) {
    if (datum == _hi) return size() - 1; // Make last bin inclusive of boundary

    return (datum - _lo)/_delta; // TODO May need to be explicitly rounded if compiler implementation is weird?
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

size_t Histogram::n_data() {
    return std::reduce(begin(), end());
}

size_t Histogram::max_bin() {
    return *std::max_element(begin(), end());
}

scalar Histogram::range() {
    return _hi - _lo;
}

scalar Histogram::mean_rate() {
    return n_data() / range();
}

std::string Histogram::display() {
    std::string out = "[";
    for (size_t i = 0; i < size(); i++) {
        if (i != 0) {
            out += ", ";
        }
        out += std::to_string(operator[](i));
    }
    out += "]";
    return out;
}