#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "core.hpp"

#include<vector>
#include<string>
#include<iostream>

class Histogram {
    private:
        scalar hi;
        scalar lo;
        scalar delta;
        std::vector<size_t> bins;
        size_t n_data;

        size_t binOf(scalar datum);

    public:
        Histogram(size_t n_bins, scalar lo, scalar hi);
        Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data);
        // TODO define safe copy-constructor to prevent list sharing between histograms

        inline scalar get_hi() { return hi; }
        inline scalar get_lo() { return lo; }
        inline scalar get_delta() { return delta; }
        inline size_t size() { return bins.size(); }
        inline size_t get_bin(size_t n) { return bins[n]; }
        inline size_t get_n_data() { return n_data; }
        inline size_t inc_bin(size_t n) {
            ++n_data;
            return ++bins[n];
        }

        size_t max_bin();
        scalar range();
        scalar mean_rate();

        bool add_datum(scalar datum);
        size_t add_data(std::vector<scalar> data);

        std::string display();
};

#endif