/**
 * @file
 *
 * Implementation of the PlantTypeConfig struct
 */

#include "PollinatorStructs.h"
#include "PlantTypeConfig.h"

Wavelength PlantTypeConfig::getDominantWavelength() const {
    if (flowerVisDataPtr != nullptr) {
        return flowerVisDataPtr->lambda;
    }
    else {
        throw std::runtime_error("PlantTypeConfig::getDominantWavelength called with no VisStimulusInfo defined");
    }
}
