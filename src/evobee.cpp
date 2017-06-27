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
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::ifstream;
using std::vector;
using json = nlohmann::json;


// forward declaration of functions in this file
void processConfigOptions(int argc, char **argv);
void processJsonFile(ifstream& ifs);


// a helper function for reading in parameters from the JSON file and dealing
// with missing elements
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
    json_read_param(j, sct, "migration-allowed", hc.migrationAllowed);
    json_read_param(j, sct, "migration-prob", hc.migrationProb);

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
    json_read_param(j, sct, "refuge", pc.refuge);
    json_read_param(j, sct, "refuge-alien-inflow-prob", pc.refugeAlienInflowProb);
    json_read_param(j, sct, "seed-outflow-allowed", pc.seedOutflowAllowed);
    json_read_param(j, sct, "seed-outflow-prob", pc.seedOutflowProb);
}

void from_json(const json& j, PlantTypeConfig& pt)
{
    std::string sct = "PlantTypeConfig";
    json_read_param(j, sct, "species", pt.species);
    json_read_param(j, sct, "flower-reflectance-mp-init-min", pt.flowerMPInitMin);
    json_read_param(j, sct, "flower-reflectance-mp-init-max", pt.flowerMPInitMax);
    json_read_param(j, sct, "anther-init-pollen", pt.antherInitPollen);
    json_read_param(j, sct, "anther-pollen-transfer-per-visit", pt.antherPollenTransferPerVisit);
    json_read_param(j, sct, "nectar-reward", pt.nectarReward);
    json_read_param(j, sct, "stigma-max-pollen-capacity", pt.stigmaMaxPollenCapacity);
    if (pt.stigmaMaxPollenCapacity < 1)
    {
        // a user supplied value of 0 or negative means that the maximum capacity
        // is set to an effectively unlimited value
        pt.stigmaMaxPollenCapacity = 99999;
    }    
    json_read_param(j, "PlantTypeConfig", "pollen-clogging", pt.pollenClogging);
}

void from_json(const json& j, PollinatorConfig& p)
{
    std::string sct = "PollinatorConfig";
    json_read_param(j, sct, "species", p.species);
    json_read_param(j, sct, "bout-length", p.boutLength);
    json_read_param(j, sct, "pollen-loss-on-flower", p.pollenLossOnFlower);
    json_read_param(j, sct, "pollen-loss-in-air", p.pollenLossInAir);
    json_read_param(j, sct, "pollen-carryover-num-visits", p.pollenCarryoverNumVisits);
    json_read_param(j, sct, "max-pollen-capacity", p.maxPollenCapacity);
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
        int envSizeX = -1, envSizeY = -1;
        string config_file;

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "display program version number")
            ("help,h", "display this help message")
            ("config,c", po::value<string>(&config_file)->default_value("evobee.cfg.json"), "configuration file");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("env-size", po::value<int>(), "environment size for both x and y")
            ("env-size-x", po::value<int>(), "environment size in x direction")
            ("env-size-y", po::value<int>(), "environment size in y direction")
            ("vis", po::value<bool>(), "show realtime visualisation of run")
            ("vis-update-period", po::value<int>(), "number of model steps between each visualisation update")
            ("max-screen-frac", po::value<float>(), "max fraction of screen width or height for vis window")
            ("max-screen-frac-w", po::value<float>(), "max fraction of screen width for vis window")
            ("max-screen-frac-h", po::value<float>(), "max fraction of screen height for vis window")
            ("hive", po::value<vector<string>>()->composing(), "hive specification");
        //("optimization", po::value<int>(&opt)->default_value(10), "optimization level")
        //("include-path,I", po::value<vector<string>>()->composing(), "include path");

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        //hidden.add_options()("input-file", po::value<vector<string>>(), "input file");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::positional_options_description p;
        //p.add("input-file", -1);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help"))
        {
            cout << visible << endl;
            exit(0);
        }

        if (vm.count("version"))
        {
            cout << "EvoBee version " << evobee_VERSION_MAJOR << "." << evobee_VERSION_MINOR << "."
                 << evobee_VERSION_PATCH << "." << evobee_VERSION_TWEAK << endl;
            exit(0);
        }

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            /*
            std::cout << "Unable to open config file: " << config_file << std::endl;
            return 0;
            */
        }
        else
        {
            processJsonFile(ifs);
        }

        if (vm.count("env-size"))
        {
            envSizeX = envSizeY = vm["env-size"].as<int>();
        }
        if (vm.count("env-size-x"))
        {
            envSizeX = vm["env-size-x"].as<int>();
            if (vm.count("env-size"))
            {
                cerr << "Warning: specification of env-size-x overrides that of env-size" << endl;
            }
        }
        if (vm.count("env-size-y"))
        {
            envSizeY = vm["env-size-y"].as<int>();
            if (vm.count("env-size"))
            {
                cerr << "Warning: specification of env-size-y overrides that of env-size" << endl;
            }
        }

        ModelParams::setEnvSize(envSizeX, envSizeY);

        cout << "Environment size set to (" << ModelParams::getEnvSizeX() << "," << ModelParams::getEnvSizeY() << ")" << endl;

        if (vm.count("vis"))
        {
            ModelParams::setVisualisation(vm["vis"].as<bool>());
        }

        cout << (ModelParams::getVisualisation() ? "Using" : "Not using")
            << " visualisation" << endl;

        if (vm.count("vis-update-period"))
        {
            ModelParams::setVisUpdatePeriod(vm["vis-update-period"].as<int>());
        }        

        if (vm.count("max-screen-frac"))
        {
            ModelParams::setMaxScreenFrac(vm["max-screen-frac"].as<float>());
        }
        if (vm.count("max-screen-frac-w"))
        {
            ModelParams::setMaxScreenFracW(vm["max-screen-frac-w"].as<float>());
        }
        if (vm.count("max-screen-frac-h"))
        {
            ModelParams::setMaxScreenFracH(vm["max-screen-frac-h"].as<float>());
        }
        if (vm.count("hive"))
        {
            vector<string> hives = vm["hive"].as<vector<string>>();
            cout << "Hives specified:" << endl;
            for (string hive : hives)
            {
                cout << hive << endl;
            }
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        exit(0);
    }

    ModelParams::setInitialised();
}

void processJsonFile(ifstream& ifs)
{
    json& j = ModelParams::getJson();

    try
    {
        ifs >> j;
    }
    catch (json::parse_error &e)
    {
        cerr << "Parse error: " << e.what() << endl;
        exit(1);
    }
    catch (json::exception &e)
    {
        cerr << "JSON error: " << e.what() << endl;
        exit(1);
    }

    try
    {
        auto itSP = j.find("SimulationParams");
        if ((itSP != j.end()) && (itSP->is_object())) 
        {
            cout << "~~~~~ Simulation Params ~~~~~" << endl;
            for (json::iterator it = itSP->begin(); it != itSP->end(); ++it)
            {
                if (it.key() == "sim-termination-num-gens" && it.value().is_number()) {
                    cout << "Simulation termination num gens -> " << it.value() << endl;
                    ModelParams::setSimTerminationNumGens(it.value());
                }
                else if (it.key() == "generation-termination-type" && it.value().is_string()) {
                    cout << "Generation termination type -> '" << it.value() << "'" << endl;
                    ModelParams::setGenTerminationType(it.value());
                }
                else if (it.key() == "generation-termination-param" && it.value().is_number_integer()) {
                    cout << "Generation termination param (int) -> '" << it.value() << "'" << endl;
                    ModelParams::setGenTerminationIntParam(it.value());
                }
                else if (it.key() == "generation-termination-param" && it.value().is_number_float()) {
                    cout << "Generation termination param (float) -> '" << it.value() << "'" << endl;
                    ModelParams::setGenTerminationFloatParam(it.value());
                }
                else if (it.key() == "rng-seed" && it.value().is_string()) {
                    cout << "Seed -> '" << it.value() << "'" << endl;
                    ModelParams::setRngSeedStr(it.value());
                }
                else if (it.key() == "logging" && it.value().is_boolean()) {
                    cout << "Logging -> '" << it.value() << "'" << endl;
                    ModelParams::setLogging(it.value());
                }
                else if (it.key() == "log-dir" && it.value().is_string()) {
                    cout << "Log dir -> '" << it.value() << "'" << endl;
                    ModelParams::setLogDir(it.value());
                }
                else if (it.key() == "log-run-name" && it.value().is_string()) {
                    cout << "Log run name -> '" << it.value() << "'" << endl;
                    ModelParams::setLogRunName(it.value());
                }
                else if (it.key() == "visualisation" && it.value().is_boolean()) {
                    cout << "Vis -> " << it.value() << endl;
                    ModelParams::setVisualisation(it.value());
                }
                else if (it.key() == "vis-update-period" && it.value().is_number()) {
                    cout << "Vis update period -> " << it.value() << endl;
                    ModelParams::setVisUpdatePeriod(it.value());
                }
                else if (it.key() == "vis-delay-per-frame" && it.value().is_number()) {
                    cout << "Vis delay per frame (ms) -> " << it.value() << endl;
                    ModelParams::setVisDelayPerFrame(it.value());
                }                
                else if (it.key() == "vis-pollinator-trails" && it.value().is_boolean()) {
                    cout << "Vis pollinator trails -> " << it.value() << endl;
                    ModelParams::setVisPollinatorTrails(it.value());
                }
                else if (it.key() == "vis-max-screen-frac-w" && it.value().is_number()) {
                    cout << "Vis max screen fraction W -> " << it.value() << endl;
                    ModelParams::setMaxScreenFracW(it.value());
                }
                else if (it.key() == "vis-max-screen-frac-h" && it.value().is_number()) {
                    cout << "Vis max screen fraction H -> " << it.value() << endl;
                    ModelParams::setMaxScreenFracH(it.value());
                }
                else if (it.key() == "log-update-period" && it.value().is_number()) {
                    cout << "Log update period -> " << it.value() << endl;
                    ModelParams::setLogUpdatePeriod(it.value());
                }
                else {
                    cerr << "Unexpected entry in SimulationParams section of json file: "
                    << it.key() << " : " << it.value() << endl;
                }
            }
        }

        auto itEnv = j.find("Environment");
        if ((itSP != j.end()) && (itSP->is_object()))
        {
            cout << "~~~~~ Environment Params ~~~~~" << endl;
            for (json::iterator it = itEnv->begin(); it != itEnv->end(); ++it)
            {
                if (it.key() == "env-size-x" && it.value().is_number()) {
                    cout << "Env size x -> " << it.value() << endl;
                    ModelParams::setEnvSizeX(it.value());
                }
                else if (it.key() == "env-size-y" && it.value().is_number()) {
                    cout << "Env size y -> " << it.value() << endl;
                    ModelParams::setEnvSizeY(it.value());
                }
                else if (it.key() == "default-ambient-temp" && it.value().is_number()) {
                    cout << "Default ambient temp -> " << it.value() << endl;
                    ModelParams::setEnvDefaultAmbientTemp(it.value());
                }
                else if (it.key() == "background-reflectance-mp" && it.value().is_number()) {
                    cout << "Default background reflectance MP -> " << it.value() << endl;
                    ModelParams::setEnvBackgroundReflectanceMP(it.value());
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
                            cerr << "Unexpected entry in Hives section of json file: "
                            << itHives.key() << " : " << itHives.value() << endl;
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
                            cerr << "Unexpected entry in PlantTypeDistributions section of json file: "
                            << itPTDs.key() << " : " << itPTDs.value() << endl;
                        }                    
                    }
                }
                else {
                    cerr << "Unexpected entry in Environment section of json file: "
                    << it.key() << " : " << it.value() << endl;
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
                    PlantTypeConfig pc = itPT.value();
                    ModelParams::addPlantTypeConfig(pc);
                }
                else {
                    cerr << "Unexpected entry in PlantTypes section of json file: "
                    << itPT.key() << " : " << itPT.value() << endl;
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
                    PollinatorConfig pc = itP.value();
                    ModelParams::addPollinatorConfig(pc);
                }
                else {
                    cerr << "Unexpected entry in Pollinators section of json file: "
                    << itP.key() << " : " << itP.value() << endl;
                }                    
            }            
        }           

    }
    catch (json::exception &e)
    {
        cerr << "Unexpected error when reading JSON file : " << e.what() << endl;
        exit(1);
    }
}