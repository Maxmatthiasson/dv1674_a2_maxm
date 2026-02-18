/*
Author: David Holmqvist <daae19@student.bth.se>
*/

// Added x_mag and y_mag as parameters to pearson().

/*
Optimization v3:
- pearson() uses reference instead of value.
*/

#include "vector.hpp"
#include <vector>

#if !defined(ANALYSIS_HPP)
#define ANALYSIS_HPP

namespace Analysis {
std::vector<double> correlation_coefficients(std::vector<Vector> datasets);
double pearson(Vector& x_mm, Vector& y_mm, double x_mag, double y_mag);
};

#endif
