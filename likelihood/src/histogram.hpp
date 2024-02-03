#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "core.hpp"

#include<vector>

class Histogram {
    private:
        scalar hi;
        scalar lo;
        scalar delta;
        size_t n_bins;
        std::vector<size_t> bins;

        size_t binOf(scalar datum);

    public:
        Histogram(size_t n_bins, scalar lo, scalar hi);
        Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data);

        inline scalar getHi() { return hi; }
        inline scalar getLo() { return lo; }
        inline scalar getDelta() { return delta; }
        inline size_t getNBins() { return n_bins; }
        inline scalar getBin(size_t n) { return bins[n]; }      

        void addDatum(scalar datum);
        void addData(std::vector<scalar> data);
};

#endif