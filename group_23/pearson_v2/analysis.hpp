/*
Author: David Holmqvist <daae19@student.bth.se>
*/

// Added x_mag and y_mag as parameters to pearson().

#include "vector.hpp"
#include <vector>

#if !defined(ANALYSIS_HPP)
#define ANALYSIS_HPP

namespace Analysis {
std::vector<double> correlation_coefficients(std::vector<Vector> datasets);
double pearson(Vector vec1, Vector vec2, double x_mag, double y_mag);
};

#endif
