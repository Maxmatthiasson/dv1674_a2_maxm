/*
Author: David Holmqvist <daae19@student.bth.se>
*/

/*
Optimization v2:
- Pre-computing the mean for all datasets and the magnitued in the 
  correlation_coefficients() function.
- pearson() now takes magnitudes as parameters. 
- This reduces the number of functions calls to mean() and magnitude().

Optimization v3:
- Using in-place vector operations to avoid the overhead of memory allocation
  and copying.
- Passing vectors by reference instead of by value to pearson().
*/

#include "analysis.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace Analysis {

std::vector<double> correlation_coefficients(std::vector<Vector> datasets)
{
    std::vector<double> result{};
    const auto n = datasets.size();

    // Precomputing means for all datasets
    std::vector<double> means(n);
    for (auto i = 0u; i < n; i++) {
        means[i] = datasets[i].mean();
    }

    // Precomputing magnitudes
    std::vector<double> magnitudes(n);
    for (auto i = 0u; i < n; i++) {
        Vector x_mm = datasets[i];
        x_mm -= means[i]; // In-place subtraction
        magnitudes[i] = x_mm.magnitude();
    }

    for (auto sample1 = 0u; sample1 < n - 1; sample1++) {
        for (auto sample2 = sample1 + 1; sample2 < n; sample2++) {
            Vector x_mm = datasets[sample1];
            Vector y_mm = datasets[sample2];
            
            // Using in-place subtraction
            x_mm -= means[sample1];
            y_mm -= means[sample2];

            double r = pearson(x_mm, y_mm, magnitudes[sample1], magnitudes[sample2]);
            result.push_back(r);
        }
    }

    return result;
}

// Now passes vectors by reference instead of by value
double pearson(Vector& x_mm, Vector& y_mm, double x_mag, double y_mag)
{
    // In-place division
    x_mm /= x_mag;
    y_mm /= y_mag;

    double r = x_mm.dot(y_mm);
    return std::max(std::min(r, 1.0), -1.0);
}

}


