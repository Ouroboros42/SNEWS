#ifndef CORE_H
#define CORE_H

#include<vector>

typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

scalar max(vec values);
scalar min(vec values);

#endif