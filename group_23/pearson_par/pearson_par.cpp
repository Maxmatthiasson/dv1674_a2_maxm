// Seperate version where correlation_coefficients takes threads as a parameter.

#include "analysis_par.hpp"
#include "dataset.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [dataset] [outfile] [num_threads]" << std::endl;
        std::exit(1);
    }

    auto datasets = Dataset::read(argv[1]);
    int threads = std::stoi(argv[3]);

    auto corrs = Analysis::correlation_coefficients(datasets, threads);
    Dataset::write(corrs, argv[2]);

    return 0;
}
