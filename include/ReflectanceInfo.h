/**
 * @file
 *
 * Declaration of the ReflectanceInfo class
 */

#ifndef _REFLECTANCEINFO_H
#define _REFLECTANCEINFO_H

using MarkerPoint = unsigned short;
using Wavelength = unsigned short;

constexpr MarkerPoint NO_MARKER_POINT = 0;

class VisualStimulusInfo;

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
    ReflectanceInfo() : m_MarkerPoint(NO_MARKER_POINT), m_pVisDataPtr(nullptr) {};
    ReflectanceInfo(const ReflectanceInfo& other) : m_MarkerPoint(other.m_MarkerPoint), m_pVisDataPtr(other.m_pVisDataPtr) {};
    //ReflectanceInfo(MarkerPoint mp) : m_MarkerPoint(mp) {};
    ReflectanceInfo(MarkerPoint mp, const VisualStimulusInfo* pVSI) : m_MarkerPoint(mp), m_pVisDataPtr(pVSI) {};

    bool hasMarkerPoint() const {return m_MarkerPoint != NO_MARKER_POINT;}
    MarkerPoint getMarkerPoint() const {return m_MarkerPoint;}
    void setMarkerPoint(MarkerPoint mp) {m_MarkerPoint = mp;}

    const VisualStimulusInfo* getVisDataPtr() const {return m_pVisDataPtr;}
    void setVisDataPtr(const VisualStimulusInfo* pVSI) {m_pVisDataPtr = pVSI;}

    Wavelength getCharacteristicWavelength() const;

    void reset() {m_MarkerPoint = NO_MARKER_POINT; m_pVisDataPtr = nullptr;}

private:
    MarkerPoint m_MarkerPoint; ///< This is only used when ColourSystem == REGULAR_MARKER_POINTS

    const VisualStimulusInfo* m_pVisDataPtr; ///< A (non-owning) pointer to the vis-data entry associated with this flower's stimulus.
                                             ///<   This is initialised in the Flower's constructor, using data from
                                             ///<   the PlantTypeConfig struct (which is initialised in
                                             ///<   ModelParams::pairPlantTypeConfigsToVisData()).
                                             ///<   This is only used when ColourSystem == ARBITRARY_DOMINANT_WAVELENGTHS


};

#endif /* _REFLECTANCEINFO_H */
