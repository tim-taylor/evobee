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
#include "EvoBeeExperiment.h"
#include "HiveConfig.h"
#include "PlantTypeDistributionConfig.h"

/*
#include <random>
#include <iomanip>
*/

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


// helper functions for JSON conversion
void from_json(const json& j, HiveConfig& hc) {
    hc.type = j.at("pollinator-type").get<string>();
    hc.num = j.at("pollinator-number").get<int>();
    hc.x = j.at("pos-x").get<int>();
    hc.y = j.at("pos-y").get<int>();
}

void from_json(const json& j, PlantTypeDistributionConfig& pc) {
    pc.plantType = j.at("plant-type").get<string>();
    pc.distribution = j.at("distribution").get<string>();
    pc.density = j.at("density").get<float>();
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
        EvoBeeExperiment expt;
        expt.run();
    }
    catch (exception &e)
    {
        cerr << "Unhandled exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}

/////////////////////////////////////////////
// normal_distribution
/*
    const int nrolls = 1000000; // number of experiments
    const int nstars = 700;   // maximum number of stars to distribute

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(5.0, 2.0);

    int p[10] = {};

    for (int i = 0; i < nrolls; ++i)
    {
        double number = distribution(generator);
        if ((number >= 0.0) && (number < 10.0))
            ++p[int(number)];
    }

    std::cout << "normal_distribution (5.0,2.0):" << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        std::cout << std::setw(2) << i << "-" << std::setw(2) << (i + 1) << ": ";
        std::cout << std::string(p[i] * nstars / nrolls, '*') << std::endl;
    }
    */
////////////////////////////

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
            //store(parse_config_file(ifs, config_file_options), vm);
            //notify(vm);
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
    json j;

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
                if (it.key() == "visualisation" && it.value().is_boolean()) {
                    cout << "Vis -> " << it.value() << endl;
                    ModelParams::setVisualisation(it.value());
                }
                else if (it.key() == "rng-seed" && it.value().is_number()) {
                    cout << "Seed -> " << it.value() << endl;
                    ModelParams::setRngSeed(it.value());
                }
                else if (it.key() == "termination-num-steps" && it.value().is_number()) {
                    cout << "Num steps -> " << it.value() << endl;
                    ModelParams::setTerminationNumSteps(it.value());
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
                else if (it.key() == "default-background-colour" && it.value().is_string()) {
                    cout << "Default background colour -> " << it.value() << endl;
                    ModelParams::setEnvDefaultBackgroundColour( Colour(it.value().get<std::string>()) );
                }
                else if (it.key() == "Hives" && it.value().is_object()) {
                    for (json::iterator itHives = it->begin(); itHives != it->end(); ++itHives)
                    {
                        if (itHives.key() == "Hive" && itHives.value().is_object()) {
                            HiveConfig hc = itHives.value();
                            ModelParams::addHiveConfig(hc);
                            // NB for creating Pollinators, Flowers etc of right type,
                            // use factory pattern:
                            // https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Factory
                        }
                        else {
                            cerr << "Unexpected entry in Hives section of json file: "
                            << itHives.key() << " : " << itHives.value() << endl;
                        }                    
                    }
                }
                else if (it.key() == "PlantTypeDistributions" && it.value().is_object()) {
                    for (json::iterator itPTDs = it->begin(); itPTDs != it->end(); ++itPTDs)
                    {
                        if (itPTDs.key() == "PlantTypeDistribution" && itPTDs.value().is_object()) {
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

    }
    catch (json::exception &e)
    {
        cerr << "Unexpected error when reading JSON file : " << e.what() << endl;
        exit(1);
    }
}