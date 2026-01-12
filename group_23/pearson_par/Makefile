# Author: David Holmqvist <daae19@student.bth.se>
# Added -pg for Gprof
# Changed g++-13 to g++ since I don't have g++13 and the other makefile used g++
# Added -pthread
# Added a binary for parallel version called pearson_par

CXX=g++
CXXFLAGS=-std=c++17 -g -Wunused -Wall -Wunused -pg -pthread

all: pearson pearson_par verify

# Sequential Pearson binary
pearson: vector.o dataset.o analysis.o pearson.cpp
	$(CXX) $(CXXFLAGS) pearson.cpp vector.o dataset.o analysis.o -o pearson

# Parallel Pearson binary
pearson_par: vector.o dataset.o analysis_par.o pearson_par.cpp
	$(CXX) $(CXXFLAGS) pearson_par.cpp vector.o dataset.o analysis_par.o -o pearson_par

# Analysis object files
analysis.o: analysis.hpp analysis.cpp vector.hpp
	$(CXX) $(CXXFLAGS) -c analysis.cpp -o analysis.o

analysis_par.o: analysis_par.hpp analysis_par.cpp vector.hpp
	$(CXX) $(CXXFLAGS) -c analysis_par.cpp -o analysis_par.o

dataset.o: dataset.hpp dataset.cpp vector.hpp
	$(CXX) $(CXXFLAGS) -c dataset.cpp -o dataset.o

vector.o: vector.hpp vector.cpp
	$(CXX) $(CXXFLAGS) -c vector.cpp -o vector.o

# Verification program
verify: verify.c
	$(CC) verify.c -o verify

clean:
	rm -rf pearson pearson_par verify *.o *.dSYM 2> /dev/null
