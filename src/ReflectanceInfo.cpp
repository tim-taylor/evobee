/**
 * @file
 *
 * Implementation of the ReflectanceInfo class
 */

#include "ModelParams.h"
#include "ReflectanceInfo.h"

Wavelength ReflectanceInfo::getCharacteristicWavelength() const
{
    switch (ModelParams::getColourSystem())
    {
        case ColourSystem::REGULAR_MARKER_POINTS:{
            return m_MarkerPoint;
        }
        case ColourSystem::ARBITRARY_DOMINANT_WAVELENGTHS:{
            if (m_pVisDataPtr != nullptr) {
                return m_pVisDataPtr->lambda;
            }
            else {
                return NO_MARKER_POINT;
            }
        }
        default:{
            throw std::runtime_error("Unexpected ColourSystem encountered in ReflectanceInfo::getCharacteristicWavelength()");
        }
    }
}
