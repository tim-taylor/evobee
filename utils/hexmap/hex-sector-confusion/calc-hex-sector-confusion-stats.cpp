// Compile with:
//   g++ -o calc-hex-sector-confusion-stats calc-hex-sector-confusion-stats.cpp
//
// Run with, e.g.:
//   ./calc-hex-sector-confusion-stats aus-data-simple.csv

#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

struct flowerdata {
    flowerdata()
        : ausid(0), hexx(0.0), hexy(0.0), theta(0.0), bin(0)
    {};

    flowerdata(int _ausid, float _hexx, float _hexy, float _theta, int _bin)
        : ausid(_ausid), hexx(_hexx), hexy(_hexy), theta(_theta), bin(_bin)
    {};

    float distance(const flowerdata& other) {
        return std::sqrt(((hexx-other.hexx)*(hexx-other.hexx))+((hexy-other.hexy)*(hexy-other.hexy)));
    }

    int ausid;
    float hexx;
    float hexy;
    float theta;
    int bin;
};

std::string readFileIntoString(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}


int main() {
    // load data and construct a structured map

    std::map<int, std::vector<flowerdata>> datamap;
    const std::string filename = "aus-data-simple.csv";
    char delimiter = ',';
    std::string file_contents;

    file_contents = readFileIntoString(filename);
    std::istringstream sstream(file_contents);

    int ausid = 0;
    float hexx = 0.0;
    float hexy = 0.0;
    float theta = 0.0;
    int bin = 0;

    std::string record;
    while (std::getline(sstream, record)) {
        std::istringstream line(record);

        std::getline(line, record, delimiter); ausid = std::stoi(record);
        std::getline(line, record, delimiter); hexx = std::stof(record);
        std::getline(line, record, delimiter); hexy = std::stof(record);
        std::getline(line, record, delimiter); theta = std::stof(record);
        std::getline(line, record, delimiter); bin = std::stoi(record);

        flowerdata flower{ausid, hexx, hexy, theta, bin};
        auto it = datamap.find(bin);
        if (it != datamap.end()) {
            it->second.push_back(flower);
        }
        else {
            std::vector<flowerdata> fvec{flower};
            datamap.insert({bin, fvec});
        }
    }

    // now use the structured data to do the required calculations

    int numPossibleBins = 36;

    // for each bin
    for (auto bindata : datamap) {

        //std::cout << bindata.first << "\t" << bindata.second.size() <<  std::endl;

        float cumdist1 = 0.0, cumdist2 = 0.0;
        float meandist1 = 0.0, meandist2 = 0.0;
        int count1 = 0, count2 = 0;

        int bin = bindata.first;
        int binP1 = (bin+1) % numPossibleBins;
        int binP2 = (bin+2) % numPossibleBins;
        int binM1 = (bin-1+numPossibleBins) % numPossibleBins;
        int binM2 = (bin-2+numPossibleBins) % numPossibleBins;

        //std::cout << bin << "," << binP1 << "," << binP2 << "," << binM1 << "," << binM2 << std::endl;

        // for each flower in the bin
        for (auto& flower : bindata.second) {

            // look at bin 1 above
            auto bindataP1it = datamap.find(binP1);
            if (bindataP1it != datamap.end()) {
                for (auto& flower2 : bindataP1it->second) {
                    float distance = flower.distance(flower2);
                    cumdist1 += distance;
                    count1++;

                    //std::cout << "flower (" << flower.hexx << "," << flower.hexy << "),"
                    //<< "flower2 (" << flower2.hexx << "," << flower2.hexy << "),"
                    //<< "distance " << distance << std::endl;

                    //std::cout << bin << ",1," << distance << std::endl;
                }
            }
            // look at bin 1 below
            auto bindataM1it = datamap.find(binM1);
            if (bindataM1it != datamap.end()) {
                for (auto& flower2 : bindataM1it->second) {
                    float distance = flower.distance(flower2);
                    cumdist1 += distance;
                    count1++;
                    //std::cout << bin << ",1," << distance << std::endl;
                }
            }
            // look at bin 2 above
            auto bindataP2it = datamap.find(binP2);
            if (bindataP2it != datamap.end()) {
                for (auto& flower2 : bindataP2it->second) {
                    float distance = flower.distance(flower2);
                    cumdist2 += distance;
                    count2++;
                    //std::cout << bin << ",2," << distance << std::endl;
                }
            }
            // look at bin 2 below
            auto bindataM2it = datamap.find(binM2);
            if (bindataM2it != datamap.end()) {
                for (auto& flower2 : bindataM2it->second) {
                    float distance = flower.distance(flower2);
                    cumdist2 += distance;
                    count2++;
                    //std::cout << bin << ",2," << distance << std::endl;
                }
            }
        }

        // output the results of this bin
        meandist1 = (count1 > 0) ? cumdist1 / count1 : 0.0;
        meandist2 = (count2 > 0) ? cumdist2 / count2 : 0.0;

        std::cout << bin << "," << meandist1 << "," << count1
            << "," << meandist2 << "," << count2 << std::endl;
    }

    return 0;
}