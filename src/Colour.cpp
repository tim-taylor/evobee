/**
 * @file
 *
 * Implementation of the Colour class
 */

#include <iostream> // for testing - maybe not needed?
#include <cstdlib>  // for rand - maybe not needed in long term?
#include <cassert>
#include <map>
#include "Colour.h"

const std::map<MarkerPoint, Colour::RGB> Colour::m_sMarkerPoint2RgbMap{
    {360, {1.0, 0.0, 0.0, 255}},
    {370, {0.9, 0.0, 0.1, 255}},
    {380, {0.8, 0.0, 0.2, 255}},
    {390, {0.7, 0.0, 0.3, 255}},
    {400, {0.6, 0.0, 0.4, 255}},
    {410, {0.5, 0.0, 0.5, 255}},
    {420, {0.4, 0.0, 0.6, 255}},
    {430, {0.3, 0.0, 0.7, 255}},
    {440, {0.2, 0.0, 0.8, 255}},
    {450, {0.1, 0.0, 0.9, 255}},
    {460, {0.0, 0.0, 1.0, 255}},
    {470, {0.0, 0.1, 0.9, 255}},
    {480, {0.0, 0.2, 0.8, 255}},
    {490, {0.0, 0.3, 0.7, 255}},
    {500, {0.0, 0.4, 0.6, 255}},
    {510, {0.0, 0.5, 0.5, 255}},
    {520, {0.0, 0.6, 0.4, 255}},
    {530, {0.0, 0.7, 0.3, 255}},
    {540, {0.0, 0.8, 0.2, 255}}
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
        /// @todo
        //assert(false);
        return m_sMarkerPoint2RgbMap.begin()->second;
    }
    else
    {
        return it->second;
    }
}


/*
template <int lo, int peak, int hi>
int Colour::getIntensity(int lambda)
{
    if (lambda <= lo)
    {
        return 0;
    }
    else if (lambda <= peak)
    {
        return (255 * (lambda - lo)) / (peak - lo);
    }
    else if (lambda < hi)
    {
        return (255 * (hi - lambda)) / (peak - lambda);
    }
    else
    {
        return 0;
    }
}
*/

/*
void Colour::updateRGB()
{
    assert(m_bRgbFromMarkerPoint);

    // The following values for R, G and B intensity are based upon Fig 1 in
    // Dyer, Paulk and Reser "Colour processing in complex environments..."
    m_RGB.r = getIntensity<400, 550, 650>(m_iMarkerPoint);
    m_RGB.g = getIntensity<300, 430, 510>(m_iMarkerPoint);
    m_RGB.b = getIntensity<300, 350, 410>(m_iMarkerPoint);

    /*
    m_RGB.r = rand() % 256;
    m_RGB.g = rand() % 256;
    m_RGB.b = rand() % 256;
    * /
}
*/

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
