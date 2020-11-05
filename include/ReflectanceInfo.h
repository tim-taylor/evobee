/**
 * @file
 *
 * Declaration of the ReflectanceInfo class
 */

#ifndef _REFLECTANCEINFO_H
#define _REFLECTANCEINFO_H

using MarkerPoint = unsigned short;

constexpr MarkerPoint NO_MARKER_POINT = 0;

/**
 * Definition of the different ways in which we can treat colour stimuli in these experiments
 */
enum class ColourSystem {
    REGULAR_MARKER_POINTS,          ///< Marker points at regular intervals within a defined range
    ARBITRARY_DOMINANT_WAVELENGTHS  ///< An arbitrary collection of allowable colours defined by their dominant wavelengths
};

/**
 * The ReflectanceInfo class ...
 */
class ReflectanceInfo {

public:
    ReflectanceInfo() : m_MarkerPoint(NO_MARKER_POINT) {};
    ReflectanceInfo(const ReflectanceInfo& other) : m_MarkerPoint(other.m_MarkerPoint) {};
    ReflectanceInfo(MarkerPoint mp) : m_MarkerPoint(mp) {};

    bool hasMarkerPoint() const {return m_MarkerPoint != NO_MARKER_POINT;}
    MarkerPoint getMarkerPoint() const {return m_MarkerPoint;}
    void setMarkerPoint(MarkerPoint mp) {m_MarkerPoint = mp;}

private:
    MarkerPoint m_MarkerPoint;
};

#endif /* _REFLECTANCEINFO_H */
