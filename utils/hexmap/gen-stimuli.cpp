// Code to generate a parameterised set of 2-step reflectance spectra based
// upon the approach described in Dyer (1998) J. Comp. Physiol. A. 183:203-212
// "The colour of flowers in spectrally variable illumination and insect pollinator vision".
//
// The details of the spectra can be adjusted by changing the various global variables
// defined a the top of the file.
//
// In contrast to the original approach described by Dyer, this code shifts the reflectance
// value of all points up by an amount defined by rmin.  The multipliers defined in the 
// coefficient table (cotable) are applied to the difference between a point's value and
// rmin (see the code that calculates SctA, SctB and SctC in the main loop).
//
// The output of this program can be split into individual files suitable for passing 
// to the calc-hex-pos tool as follows:
//
//   ./gen-stimuli > stimuli-dyer-param-R5-A375-B500.csv
//   gawk -F',' '{L=$1; outfile="stimulus-"L".csv"; W=300; for (N=2; N<=NF; N++) {printf("%d,%s\n",W,$N) >> outfile; W=W+10}}' stimuli-dyer-param-R5-A375-B500.csv
//   for N in `seq 1 9`; do for L in A B C D E F G H I J K; do ./calc-hex-pos stimulus-$N$L.csv $N$L; done; done > hexmap-dyer-param-R5-A375-B500.csv
//
// This file can be compiled with the following command:
//
//   g++ -o gen-stimuli gen-stimuli.cpp -std=c++17
//

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

// table of coefficients for each set of lines
std::vector<std::tuple<std::string,float,float,float>> cotable {
    //
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
    //
    /*
    {"A", 2.0, 2.0, 0.0},
    {"B", 1.8, 2.0, 0.2},
    {"C", 1.6, 2.0, 0.4},
    {"D", 1.4, 2.0, 0.6},
    {"E", 1.2, 2.0, 0.8},
    {"F", 1.0, 2.0, 1.0},
    {"G", 0.8, 2.0, 1.2},
    {"H", 0.6, 2.0, 1.4},
    {"I", 0.4, 2.0, 1.6},
    {"J", 0.2, 2.0, 1.8},
    {"K", 0.0, 2.0, 2.0}    
    */
};

int delta = 0;     // wavelength shift

int slope1lo = 350 + delta; // wavelength at which first step starts
int slope1hi = 400 + delta; // wavelength at which first step ends
int slope2lo = 450 + delta; // wavelength at which second step starts
int slope2hi = 550 + delta; // wavelength at which second step ends

int rmin = 5;       // minimum reflectance value
int rmax = 45;      // maximum reflectance value
int rstep = 5;      // step in reflectance value between each line

float interpolate(int outEnd1, int outEnd2, int inPos, int inLo, int inHi) {
    return (float)outEnd1+((float)(inPos-inLo)/(float)(inHi-inLo))*(float)(outEnd2-outEnd1);
}

int main()
{
    for (int line = 1; line <= 9; ++line) {
        int baseSctA = rmax - (line-1)*rstep;
        int baseSctB = rmin + (line-1)*rstep;
        int baseSctC = baseSctA;
        for (auto& co : cotable) {
            auto [label, coA, coB, coC] = co;   // making use of C++17's structured bindings
            int sctA = rmin + (baseSctA-rmin)*coA;
            int sctB = rmin + (baseSctB-rmin)*coB;
            int sctC = rmin + (baseSctC-rmin)*coC;
            std::cout << line << label << ",";
            for (int n = 300; n <= 650; n += 10) {
                float v;
                if (n <= slope1lo)      v=sctA;
                else if (n < slope1hi)  v=interpolate(sctA,sctB,n,slope1lo,slope1hi);
                else if (n <= slope2lo) v=sctB;
                else if (n < slope2hi)  v=interpolate(sctB,sctC,n,slope2lo,slope2hi);
                else                    v=sctC;
                std::cout << v/100.0;
                if (n<650) std::cout << ",";
            }
            std::cout << std::endl;
        }
    }
}
