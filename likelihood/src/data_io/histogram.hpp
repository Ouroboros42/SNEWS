#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "core.hpp"

#include<vector>
#include<string>
#include<iostream>

class Histogram : public std::vector<size_t> {
    private:
        scalar _hi;
        scalar _lo;
        scalar _delta;

        size_t binOf(scalar datum);

    public:
        /* Create an empty histogram of n_bins over a range from lo to hi */
        Histogram(size_t n_bins, scalar lo, scalar hi);
        /* Create a histogram and add data to it */
        Histogram(size_t n_bins, scalar lo, scalar hi, std::vector<scalar> data);
        /* Create a histogram from preset bin counts */
        Histogram(std::vector<size_t> bins, scalar lo, scalar hi);

        /* Create an empty histogram of the same shape as like */
        static Histogram empty(Histogram like);

        inline scalar hi() { return _hi; }
        inline scalar lo() { return _lo; }
        inline scalar delta() { return _delta; }
        
        inline size_t inc_bin(size_t n) { return ++at(n); } 

        size_t n_data();
        size_t max_bin();
        scalar range();
        scalar mean_rate();

        bool add_datum(scalar datum);
        size_t add_data(std::vector<scalar> data);

        std::string display();

        Histogram subset(size_t first_bin, size_t n_bins);
};

#endif