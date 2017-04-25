#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <boost/program_options.hpp>
#include "res_path.h"
#include "EvoBeeModel.h"

/*
#include <random>
#include <iomanip>
*/

namespace po = boost::program_options;
using namespace std;


int main(int argc, char **argv)
{
    /*
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "Resource path is: " << getResourcePath() << std::endl;

    SDL_Quit();
    return 0;
    */

    // Process program options
    try
    {
        int opt;
        string config_file;

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "display program version number")
            ("help,h", "display this help message")
            ("config,c", 
                po::value<string>(&config_file)->default_value("evobee.cfg"),
                "configuration file");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()("optimization",
                             po::value<int>(&opt)->default_value(10),
                             "optimization level")("include-path,I", po::value<vector<string>>()->composing(), "include path");

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()("input-file", po::value<vector<string>>(), "input file");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help"))
        {
            cout << visible << "\n";
            return 0;
        }

        if (vm.count("version"))
        {
            cout << "Multiple sources example, version 1.0\n";
            return 0;
        }

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "can not open config file: " << config_file << "\n";
            return 0;
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }        

        /*
    if (vm.count("include-path"))
    {
        cout << "Include paths are: "
             << vm["include-path"].as<vector<string>>() << "\n";
    }

    if (vm.count("input-file"))
    {
        cout << "Input files are: "
             << vm["input-file"].as<vector<string>>() << "\n";
    }
    */

        cout << "Optimization level is " << opt << "\n";
    }
    catch (exception &e)
    {
        cout << e.what() << "\n";
        return 1;
    }

    /*
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "show this help message")
            ("compression", po::value<double>(), "set compression level");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("compression"))
        {
            cout << "Compression level was set to "
                 << vm["compression"].as<double>() << ".\n";
        }
        else
        {
            cout << "Compression level was not set.\n";
        }
    }
    catch (exception &e)
    {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        cerr << "Exception of unknown type!\n";
    }
    */

    // Initialise SDL (test)

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::string imagePath = getResourcePath("test") + "hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr)
    {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr)
    {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    EvoBeeModel ebm;

    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 2; ++i)
    {
        //First clear the renderer
        SDL_RenderClear(ren);
        //Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);
        //Update the screen
        SDL_RenderPresent(ren);

        // TT - added this otherwise image doesn't show until after the first delay..
        //First clear the renderer
        SDL_RenderClear(ren);

        //Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();


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

    return 0;
}
