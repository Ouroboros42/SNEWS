#ifndef CONVERGING_H
#define CONVERGING_H

#include "../../common/common.hpp"

/*
Evaluates the sum: Mij Vi Uj
Optimised for terms converging for large i, j
Accuracy determines minimum relative size of terms to be calculated
*/
scalar mvv_contract(mat M, vec V, vec U, scalar accuracy);

#endif