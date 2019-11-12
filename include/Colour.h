/**
 * @file
 *
 * Declaration of the Colour class
 */

#ifndef _COLOUR_H
#define _COLOUR_H

#include <string>
#include <map>

#include "ReflectanceInfo.h"

/**
 * Note that the Colour class is used mainly for visualisation purposes.
 * It provides methods for converting MarkerPoints to RGB values, and
 * also for converting many named colours to RGB values.
 * The core objects of the EvoBee model itself (pollinators, flowers, etc)
 * use the ReflectanceInfo class to hold data about reflectance/visual
 * properties. It is ReflectanceInfo, not Colour, that actually determines
 * interactions between objects in the model.
 */
class Colour {

public:
    /**
     * Constructor taking an explicit RGB value
     */
    Colour(unsigned short r, unsigned short g, unsigned short b);

    /**
     * Constructor taking a colour name (using CSS-defined colour names)
     */
    //Colour(std::string name);

    /**
     *
     */
    class RGB {
    public:
        RGB() {r=0x80; g=0x80; b=0x80;}
        RGB(unsigned short r, unsigned short g, unsigned short b) {this->r = r; this->g = g; this->b = b;}
        RGB(float r, float g, float b, float max) {this->r = max*r; this->g = max*g; this->b = max*b;}
        unsigned short r;
        unsigned short g;
        unsigned short b;
    };

    /**
     * Explicitly set the display colour (making it unlinked from Marker Point)
     */
    void setRgb(unsigned short r, unsigned short g, unsigned short b);

    /**
     * Explicitly set the display colour (making it unlinked from Marker Point)
     */
    //bool setRgbFromName(std::string name);

    /**
     *
     */
    static const RGB& getRgbFromMarkerPoint(MarkerPoint mp);

    /**
     *
     */
    const RGB& getRgb() const {return m_RGB;}

private:
    //template<int lo, int peak, int hi>
    //int getIntensity(int lambda);

    Colour::RGB m_RGB;

    static const std::map<MarkerPoint, RGB> m_sMarkerPoint2RgbMap;
    static const std::map<std::string, RGB> m_sColourName2RgbMap;
    static const Colour::RGB m_sDefaultColour;
};

#endif /* _COLOUR_H */
