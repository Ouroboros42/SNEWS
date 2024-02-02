#ifndef CONVERGING_H
#define CONVERGING_H

#include "../../common/common.hpp"

/*
Evaluates the log(sum: exp(Mij)exp(Vi)exp(Uj))
Optimised for terms converging for large i, j
rel_accuracy determines minimum relative size (compared to total) of terms to be calculated
*/
scalar log_mvv_contract(mat M, vec V, vec U, scalar rel_accuracy);

#endif