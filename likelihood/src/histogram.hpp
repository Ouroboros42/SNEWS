#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "core.hpp"

#include <iostream>

using namespace std;

template <int NBins> class Histogram {
    private:
        float hi;
        float lo;
        float delta;
        array<int, NBins> bins;

// Because this is a template class, function definitions have to be in the header file, very sinful
        int binOf(float datum) {
            if (datum == hi) return NBins - 1; // Make last bin inclusive of boundary

            return (datum - lo)/delta; // TODO May need to be explicitly rounded if compiler implementation is weird?
        }

    public:
        Histogram(float lo, float hi) : lo{lo}, hi{hi}, delta{(hi-lo)/NBins}, bins{{}} /*Initialise bins to 0*/ {}
        Histogram(float lo, float hi, vector<float> data) : Histogram(lo, hi) { addData(data); }   

        inline float getHi() { return hi; }
        inline float getLo() { return lo; }
        inline float getDelta() { return delta; }
        inline array<int, NBins> getBins() { return bins; }        

        void addData(vector<float> data) {
            for(float datum : data) {
                ++bins[binOf(datum)];
            }
        }
};
#endif