/**
 * @file
 *
 * Implementation of the Colour class
 */

#include <iostream> // for testing - maybe not needed?
#include <cstdlib>  // for rand - maybe not needed in long term?
#include <cassert>
#include "Colour.h"

const int Colour::DEFAULT_MARKER_POINT = 500;


Colour::Colour() :
    m_iMarkerPoint(Colour::DEFAULT_MARKER_POINT),
    m_bRgbFromMarkerPoint(true)
{
    updateRGB();
}

Colour::Colour(int mp) :
    m_iMarkerPoint(mp),
    m_bRgbFromMarkerPoint(true)
{
    updateRGB();
}

Colour::Colour(unsigned short r, unsigned short g, unsigned short b) :
    m_iMarkerPoint(-1),
    m_bRgbFromMarkerPoint(false)
{
    setRGB(r,g,b);
}

void Colour::setRGB(unsigned short r, unsigned short g, unsigned short b)
{
    m_RGB.r = r;
    m_RGB.g = g;
    m_RGB.b = b;
}

void Colour::setRgbFromMarkerPoint(bool linked)
{
    m_bRgbFromMarkerPoint = linked;
}

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
    */
}

///
void Colour::setMarkerPoint(int mp) {
    m_iMarkerPoint = mp;
    updateRGB();
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
