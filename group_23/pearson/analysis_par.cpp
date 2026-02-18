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

Parallelization:
- Parallelized the outer loop.
- Dividing work between threads by dataset index.
*/

#include "analysis_par.hpp"
#include <pthread.h>
#include <vector>
#include <iostream>

namespace Analysis {

// Thread data for a range of sample1 indices
struct ThreadData {
    std::vector<Vector>* datasets;
    std::vector<double>* means;
    std::vector<double>* magnitudes;
    std::vector<double>* result;
    unsigned start_sample;
    unsigned end_sample;
};

// Processes a range of sample1 indices
void* correlation_worker(void* arg) {
    auto* d = static_cast<ThreadData*>(arg);

    auto& datasets = *d->datasets;
    auto& means = *d->means;
    auto& magnitudes = *d->magnitudes;
    auto& result = *d->result;

    unsigned idx = 0;
    for (unsigned i = 0; i < d->start_sample; i++)
        idx += datasets.size() - i - 1; // Starting index in result vector

    for (unsigned sample1 = d->start_sample; sample1 < d->end_sample; sample1++) {
        for (unsigned sample2 = sample1 + 1; sample2 < datasets.size(); sample2++) {

            Vector x_mm = datasets[sample1];
            Vector y_mm = datasets[sample2];
            
            // In-place subtraction
            x_mm -= means[sample1];
            y_mm -= means[sample2];

            double r = pearson(x_mm, y_mm, magnitudes[sample1], magnitudes[sample2]);
            result[idx++] = r;
        }
    }

    return nullptr;
}

std::vector<double> correlation_coefficients(std::vector<Vector> datasets, int num_threads) {
    std::vector<double> result{};
    auto n = datasets.size();

    // Precomputing means for all datasets
    std::vector<double> means(n);
    for (auto i = 0u; i < n; i++) {
        means[i] = datasets[i].mean();
    }

    // Precomputing magnitudes
    std::vector<double> magnitudes(n);
    for (auto i = 0u; i < n; i++) {
        Vector x_mm = datasets[i];
        x_mm -= means[i]; // in-place subtraction
        magnitudes[i] = x_mm.magnitude();
    }

    // Preallocating result vector
    result.resize(n * (n - 1) / 2);

    // Launching threads
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    auto samples_per_thread = n / num_threads;
    auto remainder = n % num_threads;
    unsigned start = 0;

    for (int t = 0; t < num_threads; t++) {
        unsigned end = start + samples_per_thread + (t < remainder ? 1 : 0);
        thread_data[t] = { &datasets, &means, &magnitudes, &result, start, end };
        pthread_create(&threads[t], nullptr, correlation_worker, &thread_data[t]);
        start = end;
    }

    for (int t = 0; t < num_threads; t++)
        pthread_join(threads[t], nullptr);

    return result;
}

double pearson(Vector& x_mm, Vector& y_mm, double x_mag, double y_mag)
{
    // In-place division
    x_mm /= x_mag;
    y_mm /= y_mag;

    double r = x_mm.dot(y_mm);
    return std::max(std::min(r, 1.0), -1.0);
}

}
