# Author: David Holmqvist <daae19@student.bth.se>
# Added -pg for Gprof
# Added -pthread to enable pthread support
# Added blur_par binary file for the parallelized version

CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wunused -pthread

all: blur blur_par

# Sequential version
blur: matrix.o ppm.o filters.o blur.cpp
	$(CXX) $(CXXFLAGS) blur.cpp matrix.o ppm.o filters.o -o blur

filters.o: filters.cpp filters.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c filters.cpp -o filters.o

# Parallel version
blur_par: matrix.o ppm.o filters_par.o blur_par.cpp
	$(CXX) $(CXXFLAGS) blur_par.cpp matrix.o ppm.o filters_par.o -o blur_par

filters_par.o: filters_par.cpp filters_par.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c filters_par.cpp -o filters_par.o

# Shared
matrix.o: matrix.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c matrix.cpp -o matrix.o

ppm.o: ppm.cpp ppm.hpp
	$(CXX) $(CXXFLAGS) -c ppm.cpp -o ppm.o

clean:
	rm -rf blur blur_par *.ppm *.o *.dSYM 2> /dev/null
