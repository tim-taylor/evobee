/**
 * @file
 *
 * Implementation of the simulation's main method. This deals with reading in the configuration file
 * and setting the static variables in ModelParams accordingly, performing necessary postprocessing
 * steps and consistency checks on the configuration options, creating a new EvoBeeExperiment object
 * and calling its run() method.
 *
 * \mainpage EvoBee
 *
 * \section main Source Documentation
 * This is the homepage of the EvoBee source documentation. Use the menus above and to the left to
 * navigate around the documentation.
 *
 * \subsection userdoc User Documentation
 * General user documentation can be found at https://tim-taylor.github.io/evobee
 *
 * \subsection code Source code
 * The EvoBee source code can be found at https://github.com/tim-taylor/evobee
 *
 * \subsection authors Authors
 * Tim Taylor and Alan Dorin, Monash University, Australia
 */

#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include "json.hpp"
#include "evobeeConfig.h"
#include "ModelParams.h"
#include "EvoBeeModel.h"
#include "EvoBeeExperiment.h"
#include "HiveConfig.h"
#include "PlantTypeConfig.h"
#include "PlantTypeDistributionConfig.h"
#include "PollinatorConfig.h"
#include "ReflectanceInfo.h"
#include "Position.h"

namespace po = boost::program_options;
using json = nlohmann::json;


// forward declaration of functions in this file
void processConfigOptions(int argc, char **argv);
void processJsonFile(std::ifstream& ifs);
void extractVisDataFromPollinatorConfig(const json& j, PollinatorConfig& p);


std::string strCurrentJsonSubSctName;


// Helper function for reading in parameters from the JSON file.
// This version is for required params, and throws an exception if the parameter is
// not found in the JSON file.
template<typename T>
void json_read_param(const json& j, const std::string& section, const std::string& name, T& var)
{
    try
    {
        var = j.at(name).get<T>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << section << " does not contain a "
            << name << " element. Aborting!" << std::endl;
        exit(1);
    }
}

// Helper function for reading in parameters from the JSON file.
// This version is for optional params, and takes a default value which is used
// if no matching entry is found in the JSON file.
template<typename T>
void json_read_opt_param(const json& j, const std::string& section,
                         const std::string& name, T& var, T defaultValue)
{
    try
    {
        var = j.at(name).get<T>();
    }
    catch (const std::exception& e)
    {
        var = defaultValue;
        //jsonDefaultValuesUsed[section][strCurrentJsonSubSctName][name] = var;
    }
}

//-----------------------------------------------------------------------------
// Helper functions for JSON conversion
//
// The following from_json() functions are used by the JSON library (json.hpp)
// to enable it to read in objects of the type specified by the second argument
// of each function. See https://github.com/nlohmann/json for further details.

void from_json(const json& j, HiveConfig& hc)
{
    std::string sct = "HiveConfig";
    json_read_param(j, sct, "pollinator-type", hc.type);
    json_read_param(j, sct, "pollinator-number", hc.num);
    json_read_param(j, sct, "start-from-hive", hc.startFromHive);
    json_read_param(j, sct, "pos-x", hc.position.x);
    json_read_param(j, sct, "pos-y", hc.position.y);
    json_read_param(j, sct, "area-top-left-x", hc.areaTopLeft.x);
    json_read_param(j, sct, "area-top-left-y", hc.areaTopLeft.y);
    json_read_param(j, sct, "area-bottom-right-x", hc.areaBottomRight.x);
    json_read_param(j, sct, "area-bottom-right-y", hc.areaBottomRight.y);
    json_read_opt_param(j, sct, "migration-allowed", hc.migrationAllowed, true);
    json_read_opt_param(j, sct, "migration-restricted", hc.migrationRestricted, false);
    json_read_opt_param(j, sct, "migration-prob", hc.migrationProb, 1.0f);

}

void from_json(const json& j, PlantTypeDistributionConfig& pc)
{
    std::string sct = "PlantTypeDistributionConfig";
    json_read_param(j, sct, "species", pc.species);
    json_read_param(j, sct, "area-top-left-x", pc.areaTopLeft.x);
    json_read_param(j, sct, "area-top-left-y", pc.areaTopLeft.y);
    json_read_param(j, sct, "area-bottom-right-x", pc.areaBottomRight.x);
    json_read_param(j, sct, "area-bottom-right-y", pc.areaBottomRight.y);
    json_read_opt_param(j, sct, "density", pc.density, 0.5f);
    json_read_opt_param(j, sct, "refuge", pc.refuge, false);
    json_read_opt_param(j, sct, "refuge-alien-inflow-prob", pc.refugeAlienInflowProb, 0.0f);
    json_read_opt_param(j, sct, "seed-outflow-allowed", pc.seedOutflowAllowed, true);
    json_read_opt_param(j, sct, "seed-outflow-restricted", pc.seedOutflowRestricted, false);
    json_read_opt_param(j, sct, "seed-outflow-prob", pc.seedOutflowProb, 1.0f);
    json_read_opt_param(j, sct, "repro-local-density-constrained", pc.reproLocalDensityConstrained, false);
    json_read_opt_param(j, sct, "repro-local-density-max", pc.reproLocalDensityMax, 0.5f);
}

void from_json(const json& j, PlantTypeConfig& pt)
{
    std::string sct = "PlantTypeConfig";
    json_read_param(j, sct, "species", pt.species);
    json_read_opt_param(j, sct, "flower-reflectance-mp-init-min", pt.flowerMPInitMin, (MarkerPoint)0);
    json_read_opt_param(j, sct, "flower-reflectance-mp-init-max", pt.flowerMPInitMax, (MarkerPoint)0);
    json_read_opt_param(j, sct, "flower-reflectance-mp-init-step", pt.flowerMPInitStep, (MarkerPoint)10);
    if (pt.flowerMPInitMax < pt.flowerMPInitMin) {
        pt.flowerMPInitMax = pt.flowerMPInitMin;
    }
    if (pt.flowerMPInitStep > (pt.flowerMPInitMax - pt.flowerMPInitMin)) {
        pt.flowerMPInitStep = pt.flowerMPInitMax - pt.flowerMPInitMin;
    }
    if (pt.flowerMPInitStep < 1) {
        pt.flowerMPInitStep = 1;
    }
    json_read_opt_param(j, sct, "flower-vis-data-id", pt.flowerVisDataID, 0);
    json_read_param(j, sct, "anther-init-pollen", pt.antherInitPollen);
    json_read_param(j, sct, "anther-pollen-transfer-per-visit", pt.antherPollenTransferPerVisit);
    json_read_param(j, sct, "stigma-max-pollen-capacity", pt.stigmaMaxPollenCapacity);
    if (pt.stigmaMaxPollenCapacity < 1)
    {
        // a user supplied value of 0 or negative means that the maximum capacity
        // is set to an effectively unlimited value
        pt.stigmaMaxPollenCapacity = 999999;
    }
    json_read_param(j, sct, "pollen-clogging", pt.pollenCloggingSpecies);
    json_read_param(j, sct, "repro-seed-dispersal-global", pt.reproSeedDispersalGlobal);
    json_read_opt_param(j, sct, "repro-seed-dispersal-radius-stddev", pt.reproSeedDispersalRadiusStdDev, 1.0f);
    json_read_opt_param(j, sct, "init-nectar", pt.initNectar, 0);
    json_read_opt_param(j, sct, "diff-mp-is-diff-species", pt.diffMPIsDiffSpecies, false);
}

void from_json(const json& j, PollinatorConfig& p)
{
    std::string sct = "PollinatorConfig";
    json_read_param(j, sct, "species", p.species);
    json_read_param(j, sct, "bout-length", p.boutLength);
    json_read_opt_param(j, sct, "step-type", p.strStepType, std::string("constant"));
    json_read_opt_param(j, sct, "step-length", p.stepLength, 1.0f);
    json_read_param(j, sct, "pollen-deposit-per-flower-visit", p.pollenDepositPerFlowerVisit);
    json_read_param(j, sct, "pollen-loss-in-air", p.pollenLossInAir);
    json_read_param(j, sct, "pollen-carryover-num-visits", p.pollenCarryoverNumVisits);
    json_read_param(j, sct, "max-pollen-capacity", p.maxPollenCapacity);
    json_read_opt_param(j, sct, "constancy-type", p.strConstancyType, std::string("none"));
    json_read_opt_param(j, sct, "constancy-param", p.constancyParam, 0.5f);
    json_read_opt_param(j, sct, "foraging-strategy", p.strForagingStrategy, std::string("random"));
    json_read_opt_param(j, sct, "learning-strategy", p.strLearningStrategy, std::string("stay"));
    json_read_opt_param(j, sct, "visited-flower-memory-size", p.visitedFlowerMemorySize, (unsigned int)5);
    json_read_opt_param(j, sct, "vis-base-prob-land-target", p.visBaseProbLandTarget, 0.9f);
    json_read_opt_param(j, sct, "vis-prob-land-no-target-set-delta", p.visProbLandNoTargetSetDelta, 0.2f);
    json_read_opt_param(j, sct, "vis-prob-land-nontarget-indiv-stddev", p.visProbLandNonTargetIndivStdDev, 0.01f);
    json_read_opt_param(j, sct, "vis-prob-land-increment-on-reward", p.visProbLandIncrementOnReward, 0.01f);
    json_read_opt_param(j, sct, "vis-prob-land-decrement-on-no-reward", p.visProbLandDecrementOnNoReward, 0.01f);
    json_read_opt_param(j, sct, "vis-prob-land-decrement-on-unseen", p.visProbLandDecrementOnUnseen, 0.005f);
    json_read_opt_param(j, sct, "vis-target-exact-match-only", p.visTargetExactMatchOnly, false);
    json_read_opt_param(j, sct, "nectar-collect-per-flower-visit", p.nectarCollectPerFlowerVisit, 0);
    json_read_opt_param(j, sct, "vis-match-min-hex-distance", p.visMatchMinHexDistance, 0.05f);
    json_read_opt_param(j, sct, "vis-match-max-confidence", p.visMatchMaxConfidence, 0.95f);
    json_read_opt_param(j, sct, "vis-match-max-hex-distance", p.visMatchMaxHexDistance, 0.19f);
    json_read_opt_param(j, sct, "vis-match-min-confidence", p.visMatchMinConfidence, 0.05f);
    json_read_opt_param(j, sct, "innate-preference-type", p.strInnatePrefType, std::string("giurfa"));

    if (p.maxPollenCapacity < 1)
    {
        // a user supplied value of 0 or negative means that the maximum capacity
        // is set to an effectively unlimited value
        p.maxPollenCapacity = 99999;
    }

    // now deal with vis-data array of arrays, and associated data, if present
    extractVisDataFromPollinatorConfig(j, p);
}


void extractVisDataFromPollinatorConfig(const json& j, PollinatorConfig& p)
{
    // parse the vis-data part of the Pollinator configuration section of the config file and
    // store the data in the PollinatorConfig object
    if (j.find("vis-data") != j.end()) {
        int numEntries = 0;
        MarkerPoint mpPrev = 0;

        auto visdata = j.at("vis-data");
        if (visdata.is_array())
        {
            // we've found an array! It should be an array of arrays, so let's try iterating through it
            for (json::iterator itAA = visdata.begin(); itAA != visdata.end(); ++itAA)
            {
                if (itAA.value().is_array() && ((itAA.value().size() == 6) || (itAA.value().size() == 10)))
                {
                    // we've found an array within the array, so now read the values
                    json::iterator itV = itAA->begin();
                    try
                    {
                        int id = -1, auxid = -1;
                        MarkerPoint mp = 0;
                        float dp = 0.0f, gc = 0.0f, hexx = 0.0f, hexy = 0.0f, purex = 0.0f, purey = 0.0f, bpli = 0.0f;

                        if (itAA.value().size() == 6)
                        {
                            mp = itV.value();       // marker point
                            dp = (++itV).value();   // detection probability
                            gc = (++itV).value();   // green contast
                            hexx = (++itV).value(); // hexagonal colour space x coord
                            hexy = (++itV).value(); // hexagonal colour space y coord
                            bpli = (++itV).value(); // pollinator's base probability of landing on this mp (non target, innate)
                        }
                        else // (itAA.value().size() == 10)
                        {
                            id = itV.value();        // id for this entry (these are referred to in the PlantType configs)
                            mp = (++itV).value();    // dominant wavelength
                            dp = (++itV).value();    // detection probability
                            gc = (++itV).value();    // green contast
                            hexx  = (++itV).value(); // hexagonal colour space x coord
                            hexy  = (++itV).value(); // hexagonal colour space y coord
                            purex = (++itV).value(); // dominant wavelength pure spectral point x coord
                            purey = (++itV).value(); // dominant wavelength pure spectral point y coord
                            bpli  = (++itV).value(); // pollinator's base probability of landing on this mp (non target, innate)
                            auxid = (++itV).value(); // auxiliary id for this entry (e.g. FReD id), only used in output logs
                        }

                        numEntries++;
                        if (numEntries == 1) {
                            // This is the first entry in the array of arrays defining the visual data.
                            // So the marker point should be the lowest marker point defined in the data.
                            // For now we set the step size to 1 in case this is the only entry in the array,
                            // but we will probably reset this step size to a different value if and when we
                            // find a second entry in the array.
                            p.visDataMPMin = mp;
                            p.visDataMPStep = 1;
                        }
                        else if (numEntries == 2) {
                            // This is the second entry in the array of arrays defining the visual data.
                            // We set the step size here, calculated as the difference between this marker
                            // point and the first one. We expect all subsequent entries to increase
                            // regularly according to the same step size.
                            if (mp >= p.visDataMPMin) {
                                p.visDataMPStep = mp - p.visDataMPMin;
                            }
                            else {
                                // error! data must be in ascending order
                                std::cerr << "Error in vis-data section of config file "
                                          << "(line starting with marker point " << mp << "). " << std::endl
                                          << "Entries must appear in ascending order of wavelength. Aborting!" << std::endl;
                                exit(1);
                            }
                        }
                        else if ((numEntries > 2) && (mp - mpPrev != p.visDataMPStep)) {
                            if (mp < mpPrev) {
                                // error! data must be in ascending order
                                std::cerr << "Error in vis-data section of config file "
                                          << "(line starting with marker point " << mp << "). " << std::endl
                                          << "Entries must appear in ascending order of wavelength. Aborting!" << std::endl;
                                exit(1);
                            }
                            else {
                                p.visDataEquallySpaced = false; // we check whether this is okay during post-processing in ModelParams::checkConsistency
                            }
                        }
                        // All checks are complete and passed at this stage, so we can add the data
                        // to the visData vector
                        p.visData.emplace_back(id, mp, dp, gc, hexx, hexy, purex, purey, bpli, auxid);
                        p.visDataMPMax = mp;
                        mpPrev = mp;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Error in vis-data section of config file, in line " << itAA.value() << std::endl
                            << "Unable to parse data values. Expecting format [markerPoint, DetectionProb, GreenContrast, Xcoord, Ycoord, BaseProbLandNonTargetInnate]."  << std::endl
                            << "Aborting!" << std::endl;
                        exit(1);
                    }
                }
                else
                {
                    std::cerr << "Error in vis-data section of config file, in line " << itAA.value() << std::endl
                              << "Unable to parse data array. Expecting format [markerPoint, DetectionProb, GreenContrast, Xcoord, Ycoord]." << std::endl
                              << "Aborting!" << std::endl;
                    exit(1);
                }
            }
        }
        else
        {
            std::cerr << "Error in vis-data section of config file." << std::endl
                      << "Expected data to be presented as an array of arrays. Aborting!" << std::endl;
            exit(1);
        }

        p.visDataDefined = true;
    }
}


// End of from_json helper functions
//-----------------------------------------------------------------------------


/**
 * Process arguments from command line and/or a config file and initialise
 * ModelParams accordingly, then create an EvoBeeExperiment object and
 * call its run method.
 */
int main(int argc, char **argv)
{
    try
    {
        processConfigOptions(argc, argv);
        EvoBeeModel::seedRng();
        ModelParams::postprocess();
        ModelParams::checkConsistency();
        EvoBeeExperiment expt;
        expt.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Aborting after problem encountered: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


/**
*/
void processConfigOptions(int argc, char **argv)
{
    try
    {
        std::string config_file;
        unsigned int iTestNum = 0;

        // Declare a group of options that will be allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "display program version number")
            ("help,h", "display this help message")
            ("config,c", po::value<std::string>(&config_file)->default_value("evobee.cfg.json"), "configuration file")
            ("quiet,q", "disable verbose progress messages on stdout")
            ("test,t", po::value<unsigned int>(&iTestNum)->default_value(0), "Perform test number N instead of regular run");

        po::options_description cmdline_options;
        cmdline_options.add(generic);

        po::options_description visible("Allowed options");
        visible.add(generic);

        po::positional_options_description p;

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        // first deal with command line options that do not involve running the model
        if (vm.count("help"))
        {
            std::cout << visible << std::endl;
            exit(0);
        }

        if (vm.count("version"))
        {
            std::cout << "EvoBee version " << evobee_VERSION_MAJOR << "." << evobee_VERSION_MINOR << "."
                 << evobee_VERSION_PATCH << "." << evobee_VERSION_TWEAK << std::endl;
            exit(0);
        }

        // get the configuration file name
        std::ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            std::cerr << "Unable to open config file: " << config_file << std::endl;
            exit(1);
        }

        // is the command line explicity asking us to suppress stdout notifications?
        if (vm.count("quiet"))
        {
            ModelParams::setVerbose(false);
            ModelParams::setCommandLineQuiet(true);
        }

        if (iTestNum > 0)
        {
            ModelParams::setTestNumber(iTestNum);
        }

        // process the contents of the configuration file
        processJsonFile(ifs);

        // if command line explicitly asked for no notifications, then set that option
        // again now, in case the configuration file conflicted with this (the command line
        // flag should override what is in the config file for this)
        if (ModelParams::commandLineQuiet())
        {
            ModelParams::setVerbose(false);
        }

    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(0);
    }

    ModelParams::setInitialised();
}


void processJsonFile(std::ifstream& ifs)
{
    bool verbose = ModelParams::verbose();

    json& j = ModelParams::getJson();

    try
    {
        ifs >> j;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
        exit(1);
    }
    catch (json::exception &e)
    {
        std::cerr << "JSON error: " << e.what() << std::endl;
        exit(1);
    }

    try
    {
        auto itSP = j.find("SimulationParams");
        if ((itSP != j.end()) && (itSP->is_object()))
        {
            if (verbose) {
                std::cout << "~~~~~ Simulation Params ~~~~~" << std::endl;
            }

            for (json::iterator it = itSP->begin(); it != itSP->end(); ++it)
            {
                if (it.key() == "sim-termination-num-gens" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Simulation termination num gens -> " << it.value() << std::endl;
                    }
                    ModelParams::setSimTerminationNumGens(it.value());
                }
                else if (it.key() == "generation-termination-type" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Generation termination type -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setGenTerminationType(it.value());
                }
                else if (it.key() == "generation-termination-param" && it.value().is_number_integer()) {
                    if (verbose) {
                        std::cout << "Generation termination param (int) -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setGenTerminationIntParam(it.value());
                }
                else if (it.key() == "generation-termination-param" && it.value().is_number_float()) {
                    if (verbose) {
                        std::cout << "Generation termination param (float) -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setGenTerminationFloatParam(it.value());
                }
                else if (it.key() == "rng-seed" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Seed -> value in config file is '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setRngSeedStr(it.value());
                }
                else if (it.key() == "logging" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Logging -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogging(it.value());
                }
                else if (it.key() == "log-flags" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Log flags -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogFlags(it.value());
                }
                else if (it.key() == "log-update-period" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Log update period -> " << it.value() << std::endl;
                    }
                    ModelParams::setLogUpdatePeriod(it.value());
                }
                else if (it.key() == "log-inter-gen-update-period" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Log inter-gen update period -> " << it.value() << std::endl;
                    }
                    ModelParams::setLogInterGenUpdatePeriod(it.value());
                }
                else if (it.key() == "log-dir" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Log dir -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogDir(it.value());
                }
                else if (it.key() == "log-final-dir" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Log final dir -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogFinalDir(it.value());
                }
                else if (it.key() == "log-run-name" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Log run name -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogRunName(it.value());
                }
                else if (it.key() == "use-log-threads" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Use log threads -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setLogThreads(it.value());
                }
                else if (it.key() == "verbose" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Verbose -> " << it.value() << std::endl;
                        // NB we only set the verbose flag from the config file spec
                        // if we are not already in silent mode. If already in silent
                        // mode, that is because it has been set as such on the command
                        // line, and that should override the config file setting
                        ModelParams::setVerbose(it.value());
                    }
                }
                else if (it.key() == "visualisation" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Vis -> " << it.value() << std::endl;
                    }
                    ModelParams::setVisualisation(it.value());
                }
                else if (it.key() == "vis-update-period" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Vis update period -> " << it.value() << std::endl;
                    }
                    ModelParams::setVisUpdatePeriod(it.value());
                }
                else if (it.key() == "vis-delay-per-frame" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Vis delay per frame (ms) -> " << it.value() << std::endl;
                    }
                    ModelParams::setVisDelayPerFrame(it.value());
                }
                else if (it.key() == "vis-pollinator-trails" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Vis pollinator trails -> " << it.value() << std::endl;
                    }
                    ModelParams::setVisPollinatorTrails(it.value());
                }
                else if (it.key() == "vis-max-screen-frac-w" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Vis max screen fraction W -> " << it.value() << std::endl;
                    }
                    ModelParams::setMaxScreenFracW(it.value());
                }
                else if (it.key() == "vis-max-screen-frac-h" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Vis max screen fraction H -> " << it.value() << std::endl;
                    }
                    ModelParams::setMaxScreenFracH(it.value());
                }
                else if (it.key() == "colour-system" && it.value().is_string()) {
                    if (verbose) {
                        std::cout << "Colour system -> '" << it.value() << "'" << std::endl;
                    }
                    ModelParams::setColourSystem(it.value());
                }
                else {
                    std::cerr << "Unexpected entry in SimulationParams section of json file: "
                        << it.key() << " : " << it.value() << std::endl;
                }
            }
        }

        auto itEnv = j.find("Environment");
        if ((itSP != j.end()) && (itSP->is_object()))
        {
            if (verbose) {
                std::cout << "~~~~~ Environment Params ~~~~~" << std::endl;
            }
            for (json::iterator it = itEnv->begin(); it != itEnv->end(); ++it)
            {
                if (it.key() == "env-size-x" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Env size x -> " << it.value() << std::endl;
                    }
                    ModelParams::setEnvSizeX(it.value());
                }
                else if (it.key() == "env-size-y" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Env size y -> " << it.value() << std::endl;
                    }
                    ModelParams::setEnvSizeY(it.value());
                }
                else if (it.key() == "default-ambient-temp" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Default ambient temp -> " << it.value() << std::endl;
                    }
                    ModelParams::setEnvDefaultAmbientTemp(it.value());
                }
                else if (it.key() == "background-reflectance-mp" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Default background reflectance MP -> " << it.value() << std::endl;
                    }
                    ModelParams::setEnvBackgroundReflectanceMP(it.value());
                }
                else if (it.key() == "repro-global-density-constrained" && it.value().is_boolean()) {
                    if (verbose) {
                        std::cout << "Reproduction global density constrained -> " << it.value() << std::endl;
                    }
                    ModelParams::setReproGlobalDensityConstrained(it.value());
                }
                else if (it.key() == "repro-global-density-max" && it.value().is_number()) {
                    if (verbose) {
                        std::cout << "Reproduction global density max -> " << it.value() << std::endl;
                    }
                    ModelParams::setReproGlobalDensityMax(it.value());
                }
                else if (it.key() == "Hives" && it.value().is_object()) {
                    std::string HKey {"Hive"};
                    for (json::iterator itHives = it->begin(); itHives != it->end(); ++itHives)
                    {
                        const std::string& key = itHives.key();
                        if ((key.compare(0, HKey.size(), HKey) == 0) && // key name begins with "Hive"
                            itHives.value().is_object())
                        {
                            strCurrentJsonSubSctName = key;
                            HiveConfig hc = itHives.value();
                            ModelParams::addHiveConfig(hc);
                        }
                        else {
                            std::cerr << "Unexpected entry in Hives section of json file: "
                                << itHives.key() << " : " << itHives.value() << std::endl;
                        }
                    }
                }
                else if (it.key() == "PlantTypeDistributions" && it.value().is_object()) {
                    std::string PTDKey {"PlantTypeDistribution"};
                    for (json::iterator itPTDs = it->begin(); itPTDs != it->end(); ++itPTDs)
                    {
                        const std::string& key = itPTDs.key();
                        if ((key.compare(0, PTDKey.size(), PTDKey) == 0) && // key name begins with "PlantTypeDistribution"
                             itPTDs.value().is_object())
                        {
                            strCurrentJsonSubSctName = key;
                            PlantTypeDistributionConfig pc = itPTDs.value();
                            ModelParams::addPlantTypeDistributionConfig(pc);
                        }
                        else if (key == "auto-distribs" && itPTDs.value().is_boolean()) {
                            if (verbose) {
                                std::cout << "PTD auto distribs -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribs(itPTDs.value());
                        }
                        else if (key == "auto-distrib-num-rows" && itPTDs.value().is_number()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib num rows -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribNumRows(itPTDs.value());
                        }
                        else if (key == "auto-distrib-num-cols" && itPTDs.value().is_number()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib num cols -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribNumCols(itPTDs.value());
                        }
                        else if (key == "auto-distrib-equal-nums" && itPTDs.value().is_boolean()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib equal nums -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribEqualNums(itPTDs.value());
                        }
                        else if (key == "auto-distrib-density" && itPTDs.value().is_number()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib density -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribDensity(itPTDs.value());
                        }
                        else if (key == "auto-distrib-area-margin" && itPTDs.value().is_number()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib area margin -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribAreaMargin(itPTDs.value());
                        }
                        else if (key == "auto-distrib-regular" && itPTDs.value().is_boolean()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib regular -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribRegular(itPTDs.value());
                        }
                        else if (key == "auto-distrib-seed-outflow-allowed" && itPTDs.value().is_boolean()) {
                            if (verbose) {
                                std::cout << "PTD auto distrib seed outflow allowed -> " << itPTDs.value() << std::endl;
                            }
                            ModelParams::setPtdAutoDistribSeedOutflowAllowed(itPTDs.value());
                        }
                        else {
                            std::cerr << "Unexpected entry in PlantTypeDistributions section of json file: "
                                << itPTDs.key() << " : " << itPTDs.value() << std::endl;
                        }
                    }
                }
                else {
                    std::cerr << "Unexpected entry in Environment section of json file: "
                        << it.key() << " : " << it.value() << std::endl;
                }
            }
        }

        auto itPTs = j.find("PlantTypes");
        if ((itPTs != j.end()) && (itPTs->is_object()))
        {
            std::string PTKey {"PlantType"};
            for (json::iterator itPT = itPTs->begin(); itPT != itPTs->end(); ++itPT)
            {
                const std::string& key = itPT.key();
                if ((key.compare(0, PTKey.size(), PTKey) == 0) && // key name begins with "PlantType"
                    itPT.value().is_object())
                {
                    strCurrentJsonSubSctName = key;
                    // the following line configures the PlantTypeConfig object from
                    // the JSON object in a one-er, making use of the
                    // void from_json(const json& j, PlantTypeConfig& p) method
                    // defined earlier in the file
                    PlantTypeConfig ptc = itPT.value();
                    ModelParams::addPlantTypeConfig(ptc);
                }
                else {
                    std::cerr << "Unexpected entry in PlantTypes section of json file: "
                        << itPT.key() << " : " << itPT.value() << std::endl;
                }
            }
        }


        auto itPs = j.find("Pollinators");
        if ((itPs != j.end()) && (itPs->is_object()))
        {
            std::string PKey {"Pollinator"};
            for (json::iterator itP = itPs->begin(); itP != itPs->end(); ++itP)
            {
                const std::string& key = itP.key();
                if ((key.compare(0, PKey.size(), PKey) == 0) && // key name begins with "Pollinator"
                    itP.value().is_object())
                {
                    strCurrentJsonSubSctName = key;
                    // the following line configures the PollinatorConfig object from
                    // the JSON object in a one-er, making use of the
                    // void from_json(const json& j, PollinatorConfig& p) method
                    // defined earlier in the file
                    PollinatorConfig pc = itP.value();
                    ModelParams::addPollinatorConfig(pc);
                }
                else {
                    std::cerr << "Unexpected entry in Pollinators section of json file: "
                        << itP.key() << " : " << itP.value() << std::endl;
                }
            }
        }

    }
    catch (json::exception &e)
    {
        std::cerr << "Unexpected error when reading JSON file : " << e.what() << std::endl;
        exit(1);
    }

    /*
    for (auto& el : jsonDefaultValuesUsed)
    {
        j.push_back(el);
    }
    */
}