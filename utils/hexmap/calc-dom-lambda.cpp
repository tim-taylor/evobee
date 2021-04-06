// Code to calculate the dominant wavelength of a given reflectance spectrum.
//
// Usage:
//   calc-dom-lambda [-v] x y
// where
//   x and y give the pre-calculated position of the reflectance spectrum in hex space
//   (this can be calculated using the utility program calc-hex-pos)
//   the optional -v flag specifies verbose output
//
// Normal output:
//   The calculated dominant wavelength, or zero if the point lies in the purple area
//
// Verbose output:
//   Calculated dominant wavelength, quadrant in hex-space, ...
//     lambda1,x1,y2 and lambda2,x2,y2 of intersected segment of pure spectral line, ...
//     x,y of point of intersection
//
// This file can be compiled with the following command:
//
//   g++ -o calc-dom-lambda calc-dom-lambda.cpp -std=c++17
//

#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <iomanip>
#include <cmath>

// table of data about the pure spectral line
//    source: spectral_locus_honeybee-hex_loci_honeybee.csv,
//       see: https://docs.google.com/spreadsheets/d/1ZC61vkvGWBylBSUsmSng5A-3Fmxd9WeqDYZGyXyqbw4/edit?usp=sharing
// each line is of the following format:
//   wavelength, quadrant in hex-space (0=LL,1=UL,2=UR,3=LR), hex-space x coord, hex-space y coord, tan theta (y/x)
std::vector<std::tuple<int, int, float, float, float>> pureline {
    {320, 0, -0.60639, -0.20896, 0.3446},
    {330, 0, -0.62295, -0.20720, 0.3326},
    {340, 0, -0.64415, -0.19074, 0.2961},
    {350, 0, -0.65662, -0.16312, 0.2484},
    {360, 0, -0.67093, -0.12494, 0.1862},
    {370, 0, -0.67062, -0.04276, 0.0638},
    {380, 1, -0.64899, 0.06327, -0.0975},
    {390, 1, -0.62293, 0.17814, -0.2860},
    {400, 1, -0.56746, 0.28085, -0.4949},
    {410, 1, -0.47306, 0.36253, -0.7664},
    {420, 1, -0.25038, 0.51454, -2.0550},
    {430, 1, -0.02537, 0.61938, -24.4148},
    {440, 2, 0.14129, 0.67230, 4.7583},
    {450, 2, 0.33351, 0.65765, 1.9719},
    {460, 2, 0.50516, 0.61102, 1.2096},
    {470, 2, 0.73044, 0.53571, 0.7334},
    {480, 2, 0.79889, 0.51516, 0.6448},
    {490, 2, 0.83919, 0.49438, 0.5891},
    {500, 2, 0.85042, 0.48797, 0.5738},
    {510, 2, 0.85128, 0.46559, 0.5469},
    {520, 2, 0.85424, 0.37720, 0.4416},
    {530, 2, 0.83556, 0.07528, 0.0901},
    {540, 3, 0.79806, -0.29299, -0.3671},
    {550, 3, 0.78155, -0.39415, -0.5043},
    {560, 3, 0.76890, -0.42903, -0.5580}
};

///////////////////////////////////////////////////////////////
// The following two methods for finding the intersection of 
// two lines is a slightly modified version of code taken from
// https://rosettacode.org/wiki/Find_the_intersection_of_two_lines#C.2B.2B

/** Calculate determinant of matrix:
	[a b]
	[c d]
*/
inline float determinant(float a, float b, float c, float d)
{
	return a*d - b*c;
}
 
/// Calculate intersection of two lines.
///\return true if found, false if not found or error
bool calcLineLineIntersect(float x1, float y1, // Line 1 start
	float x2, float y2, // Line 1 end
	float x3, float y3, // Line 2 start
	float x4, float y4, // Line 2 end
	float &ixOut, float &iyOut) // Output 
{
	float detL1 = determinant(x1, y1, x2, y2);
	float detL2 = determinant(x3, y3, x4, y4);
	float x1mx2 = x1 - x2;
	float x3mx4 = x3 - x4;
	float y1my2 = y1 - y2;
	float y3my4 = y3 - y4;
 
	float denom = determinant(x1mx2, y1my2, x3mx4, y3my4);
	if(denom == 0.0) // lines don't seem to cross
	{
		ixOut = NAN;
		iyOut = NAN;
		return false;
	}
 
	float xnom = determinant(detL1, x1mx2, detL2, x3mx4);
	float ynom = determinant(detL1, y1my2, detL2, y3my4);
	ixOut = xnom / denom;	
	iyOut = ynom / denom;
	if(!std::isfinite(ixOut) || !std::isfinite(iyOut)) // probably a numerical issue
		return false;
 
	return true; // all OK
}
///////////////////////////////////////////////////////////////

float lineLength(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int main(int argc, char *argv[])
{
    /*
    float inx = 0.1, iny = 0.9;
    bool verbose = true;
    */

    // process command line arguments
    float inx = 0.0, iny = 0.0;
    bool verbose = false;
    bool args_ok = false;
    bool opt_arg_ok = true;
    int xpos = 1;
    if (argc >= 3 && argc <= 4)
    {
        if (argc == 4)
        {
            xpos = 2;
            if (std::string(argv[1]) == "-v")
            {
                verbose = true;
            }
            else
            {
                opt_arg_ok = false;
            }
        }
        std::istringstream xss{argv[xpos]};
        std::istringstream yss{argv[xpos + 1]};
        if ((xss >> inx) && (yss >> iny))
        {
            args_ok = true;
        }
    }
    if (!(args_ok && opt_arg_ok))
    {
        std::cerr << "Usage: " << argv[0] << " [-v] x y" << std::endl;
        return 1;
    }

    // a simple hack to avoid divide by zero problems
    if (inx == 0.0f) inx = 0.000001f;
    if (iny == 0.0f) iny = 0.000001f;

    // First establish which line segment (if any) on the pure spectral line the extension of the
    // line from the origin to the (inx, iny) will intersect. This is done by first establishing
    // which quadrant the point lies in, then looking at where tan theta (iny/inx) lies in relation
    // to the tan values of the line segment end points.
    int inq;
    if (inx < 0.0)
    {
        inq = (iny < 0.0) ? 0 : 1;
    }
    else
    {
        inq = (iny < 0.0) ? 3 : 2;
    }

    float intan = iny / inx;

    float segline_x1 = 0.0, segline_y1 = 0.0, segline_x2 = 0.0, segline_y2 = 0.0;
    int segline_w1 = 0, segline_w2 = 0;
    bool out_of_range = false;
    bool found = false;
    int num_pts = (int)pureline.size();

    // Now loop through the entries in the pure spectral line table to figure out where
    // the input point lies in relation to the pure spectral line segments by looking at
    // the tangent of the angle extended by the line from the origin to the point versus
    // from the origin to the ends of the line segments
    for (int i = 0; i < num_pts; ++i)
    {
        auto [w, q, x, y, t] = pureline.at(i);

        if ((q == inq) && ((i == 0 && intan > t) || (i == num_pts - 1 && intan < t)))
        {
            out_of_range = true;
            break;
        }
        if (i > 0)
        {
            auto [pw, pq, px, py, pt] = pureline.at(i - 1);

            if (pq < q)
            {
                if (((q == 1) && ((inq == 0 && intan < pt) || (inq == 1 && intan > t))) ||
                    ((q == 2) && ((inq == 1 && intan < pt) || (inq == 2 && intan > t))) ||
                    ((q == 3) && ((inq == 2 && intan < pt) || (inq == 3 && intan > t))))
                {
                    found = true;
                }
            }
            else if (q == inq)
            {
                if ((intan > pt && intan < t) || (intan < pt && intan > t))
                {
                    found = true;
                }
            }

            if (found)
            {
                segline_x1 = px;
                segline_y1 = py;
                segline_x2 = x;
                segline_y2 = y;
                segline_w1 = pw;
                segline_w2 = w;
                break;
            }
        }
    }

    if (out_of_range)
    {
        if (verbose)
        {
            std::cout << "0.0, " << inq << ", OUT_OF_RANGE" << std::endl;
        }
        else
        {
            std::cout << "0.0" << std::endl;
        }
        return 0;
    }

    if (!found) 
    {
        std::cerr << "Error! Unable to find intersecting line segment for given point (" << inx << ", " << iny << ")" << std::endl;
        return 1;
    }

    // calculate the intersection point of the line from the origin to the input point with the pure spectral line segment
    float int_x = 0.0, int_y = 0.0;
	bool ok = calcLineLineIntersect(0.0, 0.0, inx, iny, segline_x1, segline_y1, segline_x2, segline_y2, int_x, int_y);

    if (!ok)
    {
        std::cerr << "Error! Problem encountered when calculating intersection" << std::endl;
        return 1;
    }

    // now interpolate the intersection point between the start and end of the line segment to find the dominant wavelength
    float len1 = lineLength(segline_x1, segline_y1, int_x, int_y);
    float len2 = lineLength(segline_x1, segline_y1, segline_x2, segline_y2);
    float domw = (float)segline_w1 + ((float)(segline_w2-segline_w1))*(len1/len2);

    if (verbose)
    {
        std::cout << std::setprecision(7) << domw << ", " << inx << ", " << iny << ", " << inq << ", " 
                  << segline_w1 << ", " << segline_x1 << ", " << segline_y1 << ", "
                  << segline_w2 << ", " << segline_x2 << ", " << segline_y2 << ", "
                  << int_x << ", " << int_y << std::endl;
    }
    else
    {
        std::cout << std::setprecision(7) << domw << std::endl;
    }

    return 0;
}
