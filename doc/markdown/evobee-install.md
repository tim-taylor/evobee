# EvoBee installation and compilation

[[ [Return to top-level documentation menu](../../README.md) ]]


## External dependencies  
 
To compile evobee from source, you will need the following libraries  (including header files) installed on your system:  
  
- Boost libraries (http://www.boost.org/)  
	* specifically the `program_options` library  
	* version 1.32.0 or higher  
	* on Ubuntu systems, installing the `libboost-all-dev` package should give you everything you need. 
  
- SDL 2.0 (https://www.libsdl.org/)  
	* on Ubuntu systems, installing the `libsdl2-dev` package should give you everything you need
  
  
## To build EvoBee  
  
The following instructions are for the Linux command line. Adapt  
as appropriate according to your OS.  
  
From the evobee base directory:  
  
- `cd build`  
- `cmake ..`  
- `make`  
  
If all worked well, the evobee executable file should now be found  
in the `build` directory.

> Note for Linux users: should you ever need to do a completely fresh build of the system, there is a bash script named `clean` in the `build` folder, which can be run before repeating the steps listed above.


## To compile the documentation

Should you need to recompile the Doxygen auto-generated code documentation, run the following command from the evobee base directory:

 - `doxygen doxygen.cfg`

## More details

-   SDL2 and SDL2_image installed locally
    
-   To compile SDL2:
    -   ./configure --prefix=/home/ttay0006/evobee/code/sdl2    

-   To compile SDL2_image, need to do:
	- export SDL2_CONFIG=/home/ttay0006/evobee/code/sdl2/bin/sdl2-config
	-   ./configure --prefix=/home/ttay0006/evobee/code/sdl2_image
    

-   Boost:
	- export BOOST_ROOT=/home/ttay0006/evobee/code/boost/boost_1_64_0
    ./bootstrap.sh --prefix=/home/ttay0006/evobee/code/boost --with-libraries=program_options   

-   Added the following to CMakeLists.txt:
	- set(CMAKE_PREFIX_PATH "/home/ttay0006/evobee/code/sdl2")

-   To compile evobee, need to use new versions of some modules:
	- module unload boost/1.58.0
	- module load boost/1.63.0
	- module unload cmake/3.2.3
	- module load cmake/3.5.2
	- module unload gcc/4.9.1
	- module load gcc/6.1.0
	- export LD_LIBRARY_PATH=/home/ttay0006/evobee/code/sdl2/lib:$LD_LIBRARY_PATH
	- export LD_RUN_PATH="/home/ttay0006/evobee/code/sdl2/lib"
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTg2NjY0MTY5MywtMTYzMjQ3NjMxNiwtMT
I5MzUzNTQsLTYxNzUwMzcxNl19
-->