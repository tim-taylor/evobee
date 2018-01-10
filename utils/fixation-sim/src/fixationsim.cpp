/**
 * Small simulation to calculate fixation time by random drift in an environment
 * of two species which start off being present in equal numbers. The sim
 * currently uses a hard-coded total max population size of 16000.
 *
 * Compile with:
 *   g++ -o fixationsim fixationsim.cpp
 *
 * Run with:
 *   ./fixationsim [-v]
 * where -v specifies verbose output
 *
 * Tim Taylor
 * 4 January 2018
 */

#include <iostream>
#include <random>

int main(int argc, char* argv[])
{
    const int maxPop = 16000;
    int currentPopSpecies1 = maxPop / 2;
    int generation = 0;
    int newPopSpecies1 = 0;

    std::mt19937 rngEngine{std::random_device{}()};
    std::uniform_int_distribution<int> uniformProbDistrib(0, maxPop-1);

    bool verbose = false;
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-v") verbose = true;
    }

    if (verbose) {
        std::cout << "Generation " << generation << ": species 1 count = " << currentPopSpecies1 << std::endl;
    }

    do {
        // create the next generation
        newPopSpecies1 = 0;

        for (int i=0; i<maxPop; ++i) {
            if (uniformProbDistrib(rngEngine) < currentPopSpecies1) ++newPopSpecies1;
        }

        currentPopSpecies1 = newPopSpecies1;
        ++generation;

        if (verbose) {
            std::cout << "Generation " << generation << ": species 1 count = " << currentPopSpecies1 << std::endl;
        }
    }
    while ((currentPopSpecies1 > 0) && (currentPopSpecies1 < maxPop));

    std::cout << generation << " " << (currentPopSpecies1==0 ? "2" : "1") << std::endl;

    return 0;
}
