/**
 * @file
 *
 * Implementation of the Colour class
 */

#include <iostream> // for testing - maybe not needed?
#include <cstdlib>  // for rand - maybe not needed in long term?
#include <cassert>
#include <stdexcept>
#include <map>
#include "Colour.h"

const std::map<MarkerPoint, Colour::RGB> Colour::m_sMarkerPoint2RgbMap{
    {360, {1.00, 0.00, 0.00, 255}}, 
    {361, {0.99, 0.00, 0.01, 255}}, 
    {362, {0.98, 0.00, 0.02, 255}}, 
    {363, {0.97, 0.00, 0.03, 255}}, 
    {364, {0.96, 0.00, 0.04, 255}}, 
    {365, {0.95, 0.00, 0.05, 255}}, 
    {366, {0.94, 0.00, 0.06, 255}}, 
    {367, {0.93, 0.00, 0.07, 255}}, 
    {368, {0.92, 0.00, 0.08, 255}}, 
    {369, {0.91, 0.00, 0.09, 255}}, 
    {370, {0.90, 0.00, 0.10, 255}}, 
    {371, {0.89, 0.00, 0.11, 255}}, 
    {372, {0.88, 0.00, 0.12, 255}}, 
    {373, {0.87, 0.00, 0.13, 255}}, 
    {374, {0.86, 0.00, 0.14, 255}}, 
    {375, {0.85, 0.00, 0.15, 255}}, 
    {376, {0.84, 0.00, 0.16, 255}}, 
    {377, {0.83, 0.00, 0.17, 255}}, 
    {378, {0.82, 0.00, 0.18, 255}}, 
    {379, {0.81, 0.00, 0.19, 255}}, 
    {380, {0.80, 0.00, 0.20, 255}}, 
    {381, {0.79, 0.00, 0.21, 255}}, 
    {382, {0.78, 0.00, 0.22, 255}}, 
    {383, {0.77, 0.00, 0.23, 255}}, 
    {384, {0.76, 0.00, 0.24, 255}}, 
    {385, {0.75, 0.00, 0.25, 255}}, 
    {386, {0.74, 0.00, 0.26, 255}}, 
    {387, {0.73, 0.00, 0.27, 255}}, 
    {388, {0.72, 0.00, 0.28, 255}}, 
    {389, {0.71, 0.00, 0.29, 255}}, 
    {390, {0.70, 0.00, 0.30, 255}}, 
    {391, {0.69, 0.00, 0.31, 255}}, 
    {392, {0.68, 0.00, 0.32, 255}}, 
    {393, {0.67, 0.00, 0.33, 255}}, 
    {394, {0.66, 0.00, 0.34, 255}}, 
    {395, {0.65, 0.00, 0.35, 255}}, 
    {396, {0.64, 0.00, 0.36, 255}}, 
    {397, {0.63, 0.00, 0.37, 255}}, 
    {398, {0.62, 0.00, 0.38, 255}}, 
    {399, {0.61, 0.00, 0.39, 255}}, 
    {400, {0.60, 0.00, 0.40, 255}}, 
    {401, {0.59, 0.00, 0.41, 255}}, 
    {402, {0.58, 0.00, 0.42, 255}}, 
    {403, {0.57, 0.00, 0.43, 255}}, 
    {404, {0.56, 0.00, 0.44, 255}}, 
    {405, {0.55, 0.00, 0.45, 255}}, 
    {406, {0.54, 0.00, 0.46, 255}}, 
    {407, {0.53, 0.00, 0.47, 255}}, 
    {408, {0.52, 0.00, 0.48, 255}}, 
    {409, {0.51, 0.00, 0.49, 255}}, 
    {410, {0.50, 0.00, 0.50, 255}}, 
    {411, {0.49, 0.00, 0.51, 255}}, 
    {412, {0.48, 0.00, 0.52, 255}}, 
    {413, {0.47, 0.00, 0.53, 255}}, 
    {414, {0.46, 0.00, 0.54, 255}}, 
    {415, {0.45, 0.00, 0.55, 255}}, 
    {416, {0.44, 0.00, 0.56, 255}}, 
    {417, {0.43, 0.00, 0.57, 255}}, 
    {418, {0.42, 0.00, 0.58, 255}}, 
    {419, {0.41, 0.00, 0.59, 255}}, 
    {420, {0.40, 0.00, 0.60, 255}}, 
    {421, {0.39, 0.00, 0.61, 255}}, 
    {422, {0.38, 0.00, 0.62, 255}}, 
    {423, {0.37, 0.00, 0.63, 255}}, 
    {424, {0.36, 0.00, 0.64, 255}}, 
    {425, {0.35, 0.00, 0.65, 255}}, 
    {426, {0.34, 0.00, 0.66, 255}}, 
    {427, {0.33, 0.00, 0.67, 255}}, 
    {428, {0.32, 0.00, 0.68, 255}}, 
    {429, {0.31, 0.00, 0.69, 255}}, 
    {430, {0.30, 0.00, 0.70, 255}}, 
    {431, {0.29, 0.00, 0.71, 255}}, 
    {432, {0.28, 0.00, 0.72, 255}}, 
    {433, {0.27, 0.00, 0.73, 255}}, 
    {434, {0.26, 0.00, 0.74, 255}}, 
    {435, {0.25, 0.00, 0.75, 255}}, 
    {436, {0.24, 0.00, 0.76, 255}}, 
    {437, {0.23, 0.00, 0.77, 255}}, 
    {438, {0.22, 0.00, 0.78, 255}}, 
    {439, {0.21, 0.00, 0.79, 255}}, 
    {440, {0.20, 0.00, 0.80, 255}}, 
    {441, {0.19, 0.00, 0.81, 255}}, 
    {442, {0.18, 0.00, 0.82, 255}}, 
    {443, {0.17, 0.00, 0.83, 255}}, 
    {444, {0.16, 0.00, 0.84, 255}}, 
    {445, {0.15, 0.00, 0.85, 255}}, 
    {446, {0.14, 0.00, 0.86, 255}}, 
    {447, {0.13, 0.00, 0.87, 255}}, 
    {448, {0.12, 0.00, 0.88, 255}}, 
    {449, {0.11, 0.00, 0.89, 255}}, 
    {450, {0.10, 0.00, 0.90, 255}}, 
    {451, {0.09, 0.00, 0.91, 255}}, 
    {452, {0.08, 0.00, 0.92, 255}}, 
    {453, {0.07, 0.00, 0.93, 255}}, 
    {454, {0.06, 0.00, 0.94, 255}}, 
    {455, {0.05, 0.00, 0.95, 255}}, 
    {456, {0.04, 0.00, 0.96, 255}}, 
    {457, {0.03, 0.00, 0.97, 255}}, 
    {458, {0.02, 0.00, 0.98, 255}}, 
    {459, {0.01, 0.00, 0.99, 255}}, 
    {460, {0.00, 0.00, 1.00, 255}}, 
    {461, {0.00, 0.01, 0.99, 255}}, 
    {462, {0.00, 0.02, 0.98, 255}}, 
    {463, {0.00, 0.03, 0.97, 255}}, 
    {464, {0.00, 0.04, 0.96, 255}}, 
    {465, {0.00, 0.05, 0.95, 255}}, 
    {466, {0.00, 0.06, 0.94, 255}}, 
    {467, {0.00, 0.07, 0.93, 255}}, 
    {468, {0.00, 0.08, 0.92, 255}}, 
    {469, {0.00, 0.09, 0.91, 255}}, 
    {470, {0.00, 0.10, 0.90, 255}}, 
    {471, {0.00, 0.11, 0.89, 255}}, 
    {472, {0.00, 0.12, 0.88, 255}}, 
    {473, {0.00, 0.13, 0.87, 255}}, 
    {474, {0.00, 0.14, 0.86, 255}}, 
    {475, {0.00, 0.15, 0.85, 255}}, 
    {476, {0.00, 0.16, 0.84, 255}}, 
    {477, {0.00, 0.17, 0.83, 255}}, 
    {478, {0.00, 0.18, 0.82, 255}}, 
    {479, {0.00, 0.19, 0.81, 255}}, 
    {480, {0.00, 0.20, 0.80, 255}}, 
    {481, {0.00, 0.21, 0.79, 255}}, 
    {482, {0.00, 0.22, 0.78, 255}}, 
    {483, {0.00, 0.23, 0.77, 255}}, 
    {484, {0.00, 0.24, 0.76, 255}}, 
    {485, {0.00, 0.25, 0.75, 255}}, 
    {486, {0.00, 0.26, 0.74, 255}}, 
    {487, {0.00, 0.27, 0.73, 255}}, 
    {488, {0.00, 0.28, 0.72, 255}}, 
    {489, {0.00, 0.29, 0.71, 255}}, 
    {490, {0.00, 0.30, 0.70, 255}}, 
    {491, {0.00, 0.31, 0.69, 255}}, 
    {492, {0.00, 0.32, 0.68, 255}}, 
    {493, {0.00, 0.33, 0.67, 255}}, 
    {494, {0.00, 0.34, 0.66, 255}}, 
    {495, {0.00, 0.35, 0.65, 255}}, 
    {496, {0.00, 0.36, 0.64, 255}}, 
    {497, {0.00, 0.37, 0.63, 255}}, 
    {498, {0.00, 0.38, 0.62, 255}}, 
    {499, {0.00, 0.39, 0.61, 255}}, 
    {500, {0.00, 0.40, 0.60, 255}}, 
    {501, {0.00, 0.41, 0.59, 255}}, 
    {502, {0.00, 0.42, 0.58, 255}}, 
    {503, {0.00, 0.43, 0.57, 255}}, 
    {504, {0.00, 0.44, 0.56, 255}}, 
    {505, {0.00, 0.45, 0.55, 255}}, 
    {506, {0.00, 0.46, 0.54, 255}}, 
    {507, {0.00, 0.47, 0.53, 255}}, 
    {508, {0.00, 0.48, 0.52, 255}}, 
    {509, {0.00, 0.49, 0.51, 255}}, 
    {510, {0.00, 0.50, 0.50, 255}}, 
    {511, {0.00, 0.51, 0.49, 255}}, 
    {512, {0.00, 0.52, 0.48, 255}}, 
    {513, {0.00, 0.53, 0.47, 255}}, 
    {514, {0.00, 0.54, 0.46, 255}}, 
    {515, {0.00, 0.55, 0.45, 255}}, 
    {516, {0.00, 0.56, 0.44, 255}}, 
    {517, {0.00, 0.57, 0.43, 255}}, 
    {518, {0.00, 0.58, 0.42, 255}}, 
    {519, {0.00, 0.59, 0.41, 255}}, 
    {520, {0.00, 0.60, 0.40, 255}}, 
    {521, {0.00, 0.61, 0.39, 255}}, 
    {522, {0.00, 0.62, 0.38, 255}}, 
    {523, {0.00, 0.63, 0.37, 255}}, 
    {524, {0.00, 0.64, 0.36, 255}}, 
    {525, {0.00, 0.65, 0.35, 255}}, 
    {526, {0.00, 0.66, 0.34, 255}}, 
    {527, {0.00, 0.67, 0.33, 255}}, 
    {528, {0.00, 0.68, 0.32, 255}}, 
    {529, {0.00, 0.69, 0.31, 255}}, 
    {530, {0.00, 0.70, 0.30, 255}}, 
    {531, {0.00, 0.71, 0.29, 255}}, 
    {532, {0.00, 0.72, 0.28, 255}}, 
    {533, {0.00, 0.73, 0.27, 255}}, 
    {534, {0.00, 0.74, 0.26, 255}}, 
    {535, {0.00, 0.75, 0.25, 255}}, 
    {536, {0.00, 0.76, 0.24, 255}}, 
    {537, {0.00, 0.77, 0.23, 255}}, 
    {538, {0.00, 0.78, 0.22, 255}}, 
    {539, {0.00, 0.79, 0.21, 255}}, 
    {540, {0.00, 0.80, 0.20, 255}}
};

Colour::Colour(unsigned short r, unsigned short g, unsigned short b) :
    m_RGB(r,g,b)
{
}

/*
Colour::Colour(std::string name) 
{
    if (!setRgbFromName(name))
    {
        setRgb(255,0,0);
    }
}

bool Colour::setRgbFromName(std::string name)
{
    auto it = m_sColourName2RgbMap.find(name);
    if (it == m_sColourName2RgbMap.end())
    {
        return false;
    }
    else
    {
        m_RGB = it->second;
        return true;
    }
}
*/

void Colour::setRgb(unsigned short r, unsigned short g, unsigned short b)
{
    m_RGB.r = r;
    m_RGB.g = g;
    m_RGB.b = b;
}

const Colour::RGB& Colour::getRgbFromMarkerPoint(MarkerPoint mp)
{
    auto it = m_sMarkerPoint2RgbMap.find(mp);
    
    if (it == m_sMarkerPoint2RgbMap.end())
    {
        throw std::runtime_error("Unable to find RGB for requested marker point" + mp);
    }
    else
    {
        return it->second;
    }
}

// these colour mappings were obtained from http://cloford.com/resources/colours/namedcol.htm
const std::map<std::string, Colour::RGB> Colour::m_sColourName2RgbMap{
    {"lightpink", {255, 182, 193}},
    {"pink", {255, 192, 203}},
    {"crimson", {220, 20, 60}},
    {"lavenderblush", {255, 240, 245}},
    {"palevioletred", {219, 112, 147}},
    {"hotpink", {255, 105, 180}},
    {"deeppink", {255, 20, 147}},
    {"mediumvioletred", {199, 21, 133}},
    {"orchid", {218, 112, 214}},
    {"thistle", {216, 191, 216}},
    {"plum", {221, 160, 221}},
    {"violet", {238, 130, 238}},
    {"fuchsia", {255, 0, 255}},
    {"fuchsia", {255, 0, 255}},
    {"darkmagenta", {139, 0, 139}},
    {"purple", {128, 0, 128}},
    {"mediumorchid", {186, 85, 211}},
    {"darkviolet", {148, 0, 211}},
    {"darkorchid", {153, 50, 204}},
    {"indigo", {75, 0, 130}},
    {"blueviolet", {138, 43, 226}},
    {"mediumpurple", {147, 112, 219}},
    {"mediumslateblue", {123, 104, 238}},
    {"slateblue", {106, 90, 205}},
    {"darkslateblue", {72, 61, 139}},
    {"ghostwhite", {248, 248, 255}},
    {"lavender", {230, 230, 250}},
    {"blue", {0, 0, 255}},
    {"mediumblue", {0, 0, 205}},
    {"darkblue", {0, 0, 139}},
    {"navy", {0, 0, 128}},
    {"midnightblue", {25, 25, 112}},
    {"royalblue", {65, 105, 225}},
    {"cornflowerblue", {100, 149, 237}},
    {"lightsteelblue", {176, 196, 222}},
    {"lightslategray", {119, 136, 153}},
    {"slategray", {112, 128, 144}},
    {"dodgerblue", {30, 144, 255}},
    {"aliceblue", {240, 248, 255}},
    {"steelblue", {70, 130, 180}},
    {"lightskyblue", {135, 206, 250}},
    {"skyblue", {135, 206, 235}},
    {"deepskyblue", {0, 191, 255}},
    {"lightblue", {173, 216, 230}},
    {"powderblue", {176, 224, 230}},
    {"cadetblue", {95, 158, 160}},
    {"darkturquoise", {0, 206, 209}},
    {"azure", {240, 255, 255}},
    {"lightcyan", {224, 255, 255}},
    {"paleturquoise", {175, 238, 238}},
    {"aqua", {0, 255, 255}},
    {"aqua", {0, 255, 255}},
    {"darkcyan", {0, 139, 139}},
    {"teal", {0, 128, 128}},
    {"darkslategray", {47, 79, 79}},
    {"mediumturquoise", {72, 209, 204}},
    {"lightseagreen", {32, 178, 170}},
    {"turquoise", {64, 224, 208}},
    {"aquamarine", {127, 255, 212}},
    {"mediumaquamarine", {102, 205, 170}},
    {"mediumspringgreen", {0, 250, 154}},
    {"mintcream", {245, 255, 250}},
    {"springgreen", {0, 255, 127}},
    {"mediumseagreen", {60, 179, 113}},
    {"seagreen", {46, 139, 87}},
    {"honeydew", {240, 255, 240}},
    {"darkseagreen", {143, 188, 143}},
    {"palegreen", {152, 251, 152}},
    {"lightgreen", {144, 238, 144}},
    {"limegreen", {50, 205, 50}},
    {"lime", {0, 255, 0}},
    {"forestgreen", {34, 139, 34}},
    {"green", {0, 128, 0}},
    {"darkgreen", {0, 100, 0}},
    {"lawngreen", {124, 252, 0}},
    {"chartreuse", {127, 255, 0}},
    {"greenyellow", {173, 255, 47}},
    {"darkolivegreen", {85, 107, 47}},
    {"yellowgreen", {154, 205, 50}},
    {"olivedrab", {107, 142, 35}},
    {"ivory", {255, 255, 240}},
    {"beige", {245, 245, 220}},
    {"lightyellow", {255, 255, 224}},
    {"lightgoldenrodyellow", {250, 250, 210}},
    {"yellow", {255, 255, 0}},
    {"olive", {128, 128, 0}},
    {"darkkhaki", {189, 183, 107}},
    {"palegoldenrod", {238, 232, 170}},
    {"lemonchiffon", {255, 250, 205}},
    {"khaki", {240, 230, 140}},
    {"gold", {255, 215, 0}},
    {"cornsilk", {255, 248, 220}},
    {"goldenrod", {218, 165, 32}},
    {"darkgoldenrod", {184, 134, 11}},
    {"floralwhite", {255, 250, 240}},
    {"oldlace", {253, 245, 230}},
    {"wheat", {245, 222, 179}},
    {"orange", {255, 165, 0}},
    {"moccasin", {255, 228, 181}},
    {"papayawhip", {255, 239, 213}},
    {"blanchedalmond", {255, 235, 205}},
    {"navajowhite", {255, 222, 173}},
    {"antiquewhite", {250, 235, 215}},
    {"tan", {210, 180, 140}},
    {"burlywood", {222, 184, 135}},
    {"darkorange", {255, 140, 0}},
    {"bisque", {255, 228, 196}},
    {"linen", {250, 240, 230}},
    {"peru", {205, 133, 63}},
    {"peachpuff", {255, 218, 185}},
    {"sandybrown", {244, 164, 96}},
    {"chocolate", {210, 105, 30}},
    {"saddlebrown", {139, 69, 19}},
    {"seashell", {255, 245, 238}},
    {"sienna", {160, 82, 45}},
    {"lightsalmon", {255, 160, 122}},
    {"coral", {255, 127, 80}},
    {"orangered", {255, 69, 0}},
    {"darksalmon", {233, 150, 122}},
    {"tomato", {255, 99, 71}},
    {"salmon", {250, 128, 114}},
    {"mistyrose", {255, 228, 225}},
    {"lightcoral", {240, 128, 128}},
    {"snow", {255, 250, 250}},
    {"rosybrown", {188, 143, 143}},
    {"indianred", {205, 92, 92}},
    {"red", {255, 0, 0}},
    {"brown", {165, 42, 42}},
    {"firebrick", {178, 34, 34}},
    {"darkred", {139, 0, 0}},
    {"maroon", {128, 0, 0}},
    {"white", {255, 255, 255}},
    {"whitesmoke", {245, 245, 245}},
    {"gainsboro", {220, 220, 220}},
    {"lightgrey", {211, 211, 211}},
    {"silver", {192, 192, 192}},
    {"darkgray", {169, 169, 169}},
    {"gray", {128, 128, 128}},
    {"dimgray", {105, 105, 105}},
    {"black", {0, 0, 0}}
};
