/**
 * @file
 *
 * Implementation of the Colour class
 */

#include <iostream> // for testing - maybe not needed?
#include <cstdlib> // for rand - maybe not needed in long term?
#include "Colour.h"

Colour::Colour()
{
    m_iMarkerPoint = 500;
    updateRGB();
}

Colour::Colour(int mp) :
    m_iMarkerPoint(mp)
{
    updateRGB();
}

/// @todo - this should be derived from the marker point....
void Colour::updateRGB() {
    //
    m_RGB.r = rand() % 256;
    m_RGB.g = rand() % 256;
    m_RGB.b = rand() % 256;
    //
    /*
    int n = rand() % 3;
    switch (n) {
        case 0: m_RGB.r=255; m_RGB.g=1; m_RGB.b=1; break;
        case 1: m_RGB.r=1; m_RGB.g=255; m_RGB.b=1; break;
        case 2: m_RGB.r=1; m_RGB.g=1; m_RGB.b=255; break;
    }
    std::cout << "R:" << m_RGB.r << ", G:" << m_RGB.g << ", B:" << m_RGB.b << std::endl;
    */
}

/// Returns the colour in hexadecimal RRGGBBAA form.
/// Assumes an alpha value of 255
/*
unsigned long Colour::getHex() const {
    unsigned long mask = 0x00000000FFFFFFFFull;
    return mask & (((m_RGB.r & 0xff) << 24) + ((m_RGB.g & 0xff) << 16) + ((m_RGB.b & 0xff) << 8)
           + (255 & 0xff));
}
*/

///
void Colour::setMarkerPoint(int mp) {
    m_iMarkerPoint = mp;
    updateRGB();
}