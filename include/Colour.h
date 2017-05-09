/**
 * @file
 *
 * Declaration of the Colour class
 */

#ifndef _COLOUR_H
#define _COLOUR_H

/**
 * Defines a colour as conceptualised in the EvoBee model
 */
class Colour {

public:
    Colour();
    Colour(int mp);
    ~Colour() {}

    class RGB {
    public:
        RGB() {r=0x80; g=0x80; b=0x80;}
        unsigned short r;
        unsigned short g;
        unsigned short b;
    };

    int getMarkerPoint() const {return m_iMarkerPoint;}
    const RGB& getRGB() const {return m_RGB;}
    //unsigned long getHex() const;

    void setMarkerPoint(int mp); 

private:
    void updateRGB();

    int m_iMarkerPoint;
    Colour::RGB m_RGB;
};

#endif /* _COLOUR_H */
