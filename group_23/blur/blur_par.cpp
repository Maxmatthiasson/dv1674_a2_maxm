/*
Author: David Holmqvist <daae19@student.bth.se>
*/

/*
Parallelization:
- Added another argument for number of threads.
*/

#include "matrix.hpp"
#include "ppm.hpp"
#include "filters_par.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char const* argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0]
                << " [radius] [infile] [outfile] [num_threads]" << std::endl;
        std::exit(1);
    }

    PPM::Reader reader {};
    PPM::Writer writer {};

    auto m { reader(argv[2]) };
    auto radius = std::stoi(argv[1]);
    auto threads = std::stoi(argv[4]);

    auto blurred = Filter::blur(m, radius, threads);
    writer(blurred, argv[3]);

    return 0;
}
