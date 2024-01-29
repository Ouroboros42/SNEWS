#ifndef COMMON_H
#define COMMON_H

#include<array>
#include<vector>

using namespace std;

float combinations_split(int r1, int r2);

float combinations(int n, int r);

class PascalTriangle {
    private:
        // Integers represented as floats here for much greater upper bound
        vector<vector<float>> combinations;
    public:
        PascalTriangle();
        PascalTriangle(int size);
        size_t getSize();
        void expand(int newSize);
        inline float getCombinations(int n, int r);
        inline float getSplitCombinations(int r, int j);
};

#endif