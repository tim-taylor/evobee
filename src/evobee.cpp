/**
 * @file
 *
 * Implementation of the simulation's main method
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
            << name << " element! Aborting" << std::endl;
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
    }
}


// helper functions for JSON conversion
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
    json_read_param(j, sct, "density", pc.density);
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
    json_read_param(j, sct, "flower-reflectance-mp-init-min", pt.flowerMPInitMin);
    json_read_param(j, sct, "flower-reflectance-mp-init-max", pt.flowerMPInitMax);
    json_read_param(j, sct, "anther-init-pollen", pt.antherInitPollen);
    json_read_param(j, sct, "anther-pollen-transfer-per-visit", pt.antherPollenTransferPerVisit);
    //json_read_param(j, sct, "nectar-reward", pt.nectarReward);
    json_read_param(j, sct, "stigma-max-pollen-capacity", pt.stigmaMaxPollenCapacity);
    if (pt.stigmaMaxPollenCapacity < 1)
    {
        // a user supplied value of 0 or negative means that the maximum capacity
        // is set to an effectively unlimited value
        pt.stigmaMaxPollenCapacity = 99999;
    }
    json_read_param(j, sct, "pollen-clogging", pt.pollenCloggingSpecies);
    json_read_param(j, sct, "repro-seed-dispersal-global", pt.reproSeedDispersalGlobal);
    json_read_opt_param(j, sct, "repro-seed-dispersal-radius", pt.reproSeedDispersalRadius, 1.0f);

}

void from_json(const json& j, PollinatorConfig& p)
{
    std::string sct = "PollinatorConfig";
    json_read_param(j, sct, "species", p.species);
    json_read_param(j, sct, "bout-length", p.boutLength);
    json_read_param(j, sct, "pollen-deposit-per-flower-visit", p.pollenDepositPerFlowerVisit);
    json_read_param(j, sct, "pollen-loss-in-air", p.pollenLossInAir);
    json_read_param(j, sct, "pollen-carryover-num-visits", p.pollenCarryoverNumVisits);
    json_read_param(j, sct, "max-pollen-capacity", p.maxPollenCapacity);
    json_read_opt_param(j, sct, "innate-mp-pref-min", p.innateMPPrefMin, (MarkerPoint)400);
    json_read_opt_param(j, sct, "innate-mp-pref-max", p.innateMPPrefMax, (MarkerPoint)400);
    json_read_opt_param(j, sct, "constancy-type", p.strConstancyType, std::string("none"));
    json_read_opt_param(j, sct, "constancy-param", p.constancyParam, 0.5f);
    json_read_opt_param(j, sct, "foraging-strategy", p.strForagingStrategy, std::string("random"));
    json_read_opt_param(j, sct, "visited-flower-memory-size", p.visitedFlowerMemorySize, (unsigned int)5);
    if (p.maxPollenCapacity < 1)
    {
        // a user supplied value of 0 or negative means that the maximum capacity
        // is set to an effectively unlimited value
        p.maxPollenCapacity = 99999;
    }
}


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
        ModelParams::postprocess();
        ModelParams::checkConsistency();
        EvoBeeModel::seedRng();
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

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "display program version number")
            ("help,h", "display this help message")
            ("config,c", po::value<std::string>(&config_file)->default_value("evobee.cfg.json"), "configuration file")
            ("quiet,q", "disable verbose progress messages on stdout");

        po::options_description cmdline_options;
        cmdline_options.add(generic);

        po::options_description visible("Allowed options");
        visible.add(generic);

        po::positional_options_description p;

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        bool bCommandLineQuiet = false;

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
            bCommandLineQuiet = true;
        }

        // process the contents of the configuration file
        processJsonFile(ifs);

        // if command line explicitly asked for no notifications, then set that option
        // again now, in case the configuration file conflicted with this (the command line
        // flag should override what is in the config file for this)
        if (bCommandLineQuiet)
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
                        std::cout << "Seed -> '" << it.value() << "'" << std::endl;
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
                        if ((key.compare(0, HKey.size(), HKey) == 0) &&
                            itHives.value().is_object())
                        {
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
                        if ((key.compare(0, PTDKey.size(), PTDKey) == 0) &&
                             itPTDs.value().is_object())
                        {
                            PlantTypeDistributionConfig pc = itPTDs.value();
                            ModelParams::addPlantTypeDistributionConfig(pc);
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
                if ((key.compare(0, PTKey.size(), PTKey) == 0) &&
                    itPT.value().is_object())
                {
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
                if ((key.compare(0, PKey.size(), PKey) == 0) &&
                    itP.value().is_object())
                {
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
}