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
 * Defines a colour as conceptualised in the EvoBee model.
 * The Colour class contains both a Marker Point and an RGB value.
 * If the Marker Point is explicitly set (in the constructor or by using
 * the setMarkerPoint() method), then if the RGB value is requested with the 
 * getDisplayRgb() method, the returned value is derived from the Marker Point. On
 * the other hand, if the RGB value is explicitly set (in the constructor
 * or in the setDisplayRgb() method), then the getDisplayRgb method will return the
 * set value (and the Marker Point will be ignored).
 *
 * @todo this class can now be simplified... collapse RGB into the main class itself
 */
class Colour {

public:

    /**
     * Default constructor
     * Sets the Marker Point to the value defined by the constant
     * Colour::DEFAULT_MARKER_POINT
     */
    //Colour();

    /**
     * Constructor taking an explicit marker point
     */
    //Colour(int mp);

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

    /*
     *
     */
    //void setDisplayRgbLinked(bool linked);

    /**
     * Explicitly set the display colour (making it unlinked from Marker Point)
     */    
    void setRgb(unsigned short r, unsigned short g, unsigned short b);

    /**
     * Explicitly set the display colour (making it unlinked from Marker Point)
     */
    //bool setRgbFromName(std::string name);

    /*
     *
     */    
    //void setMarkerPoint(int mp); 

    /*
     *
     */
    //int getMarkerPoint() const {return m_iMarkerPoint;}

    /**
     *
     */
    static const RGB& getRgbFromMarkerPoint(MarkerPoint mp);

    /**
     *
     */    
    const RGB& getRgb() const {return m_RGB;}

private:
    //void updateRGB();

    //template<int lo, int peak, int hi>
    //int getIntensity(int lambda);

    //int m_iMarkerPoint;
    Colour::RGB m_RGB;
    //bool m_bRgbFromMarkerPoint;

    static const std::map<MarkerPoint, RGB> m_sMarkerPoint2RgbMap;
    static const std::map<std::string, RGB> m_sColourName2RgbMap;
    //static const int DEFAULT_MARKER_POINT;
};

#endif /* _COLOUR_H */
