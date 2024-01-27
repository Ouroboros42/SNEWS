#ifndef COMMON_H
#define COMMON_H

float choose(int n, int r);

class Histogram {
    private:
        int nbins;
        float hi;
        float lo;
        float * hist;
        int datsize;
    public:
        Histogram(float * data, int ndat, int inNbins, float inLo, float inHi);
};

#endif