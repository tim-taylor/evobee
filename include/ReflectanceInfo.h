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
