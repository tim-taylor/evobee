// compile with g++ -o gen-stimuli gen-stimuli.cpp -std=c++17

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

std::vector<std::tuple<std::string,float,float,float>> cotable {
    {"A", 2.0, 1.0, 0.0},
    {"B", 1.8, 1.0, 0.2},
    {"C", 1.6, 1.0, 0.4},
    {"D", 1.4, 1.0, 0.6},
    {"E", 1.2, 1.0, 0.8},
    {"F", 1.0, 1.0, 1.0},
    {"G", 0.8, 1.0, 1.2},
    {"H", 0.6, 1.0, 1.4},
    {"I", 0.4, 1.0, 1.6},
    {"J", 0.2, 1.0, 1.8},
    {"K", 0.0, 1.0, 2.0}
};

int slope1lo = 350;
int slope1hi = 400;
int slope2lo = 450;
int slope2hi = 550;

int rmin = 5;
int rmax = 45;
int rstep = 5;

float interp(int outEnd1, int outEnd2, int inPos, int inLo, int inHi) {
    float val = 0.0;
    // TODO
    return val;
}

int main()
{
    for (int line = 1; line <= 9; line++) {
        int baseSctA = rmax - (line-1)*rstep;
        int baseSctB = rmin + (line-1)*rstep;
        int baseSctC = baseSctA;
        for (auto& co : cotable) {
            auto [label, coA, coB, coC] = co;
            int sctA = rmin + (baseSctA-rmin)*coA;
            int sctB = rmin + (baseSctB-rmin)*coB;
            int sctC = rmin + (baseSctC-rmin)*coC;
            std::cout << line << label << ",";
            for (int n = 300; n <= 650; n += 10) {
                float v;
                if (n <= slope1lo)      v=sctA;
                else if (n < slope1hi)  v=interp(sctA,sctB,n,slope1lo,slope1hi);
                else if (n <= slope2lo) v=sctB;
                else if (n < slope2hi)  v=interp(sctB,sctC,n,slope2lo,slope2hi);
                else                    v=sctC;
                std::cout << v;
                if (n<650) std::cout << ",";
            }
            std::cout << std::endl;
        }
    }
}
