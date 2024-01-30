#ifndef COMMON_H
#define COMMON_H

#include<array>
#include<vector>

using namespace std;

float combinations_split(int r1, int r2);

float combinations(int n, int r);

vector<vector<float>> pascal_rhombus(size_t n_rows, size_t n_cols);

#endif