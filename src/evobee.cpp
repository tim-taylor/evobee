#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
#include "evobeeConfig.h"
#include "ModelParams.h"
#include "EvoBeeExperiment.h"

/*
#include <random>
#include <iomanip>
*/

namespace po = boost::program_options;
using namespace std;

// forward declaration of functions in this file
void processConfigOptions(int argc, char **argv);


/**
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
            ("config,c", po::value<string>(&config_file)->default_value("evobee.cfg"), "configuration file");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("env-size", po::value<int>(), "environment size for both x and y")
            ("env-size-x", po::value<int>(), "environment size in x direction")
            ("env-size-y", po::value<int>(), "environment size in y direction")
            ("vis", po::value<bool>(), "show realtime visualisation of run");
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
            cout << "Unable to open config file: " << config_file << endl;
            return 0;
            */
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
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

        cout << (ModelParams::getVisualisation() ? "Using" : "Not using") << " visualisation" << endl;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        exit(0);
    }
}
