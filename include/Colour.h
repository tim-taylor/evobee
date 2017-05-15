/**
 * @file
 *
 * Declaration of the Colour class
 */

#ifndef _COLOUR_H
#define _COLOUR_H

/**
 * Defines a colour as conceptualised in the EvoBee model.
 * The Colour class contains both a Marker Point and an RGB value.
 * If the Marker Point is explicitly set (in the constructor or by using
 * the setMarkerPoint method), then if the RGB value is requested with the 
 * getRGB method, the returned value is derived from the Marker Point. On
 * the other hand, if the RGB value is explicitly set (in the constructor
 * or in the setRGB method), then the getRGB method will return the
 * set value (and the Marker Point will be ignored).
 */
class Colour {

public:

    /*
     * Default constructor
     * Sets the Marker Point to the value defined by the constant
     * Colour::DEFAULT_MARKER_POINT
     */
    Colour();

    /*
     * Constructor taking an explicit marker point
     */
    Colour(int mp);

    /*
     * Constructor taking an explicit RGB value
     */
    Colour(unsigned short r, unsigned short g, unsigned short b);

    /*
     *
     */
    class RGB {
    public:
        RGB() {r=0x80; g=0x80; b=0x80;}
        unsigned short r;
        unsigned short g;
        unsigned short b;
    };

    /*
     *
     */
    void setRgbFromMarkerPoint(bool linked);

    /*
     *
     */    
    void setRGB(unsigned short r, unsigned short g, unsigned short b);

    /*
     *
     */    
    void setMarkerPoint(int mp); 

    /*
     *
     */
    int getMarkerPoint() const {return m_iMarkerPoint;}

    /*
     *
     */    
    const RGB& getRGB() const {return m_RGB;}

private:
    void updateRGB();

    template<int lo, int peak, int hi>
    int getIntensity(int lambda);

    int m_iMarkerPoint;
    Colour::RGB m_RGB;
    bool m_bRgbFromMarkerPoint;

    static const int DEFAULT_MARKER_POINT;
};

#endif /* _COLOUR_H */
