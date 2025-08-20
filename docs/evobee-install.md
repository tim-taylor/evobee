# EvoBee installation and compilation

## External dependencies

To compile evobee from source, you will need the following library (including header files) installed on your system:

- Boost libraries ([http://www.boost.org/](http://www.boost.org/))
	* specifically the `program_options` library
	* version 1.32.0 or higher
	* on Ubuntu systems, installing the `libboost-program-options-dev` package should give you everything you need.

_The project used to also depend on SDL and related libraries, but the required parts of these have
now been brought into the project's repository, so there are no longer any external dependencies
for these libraries._


## To build EvoBee

The following instructions are for the Linux command line. Adapt as appropriate according to your OS.

These command should be run from the base directory of the project.

First generate the makefiles:

    > cmake -S . -B build

Then build the code:

    > cmake --build build

If all worked well, the evobee executable file should now be found in the `build/Debug` directory.

Should you ever need to do a completely fresh build of the system, use the following command to clean the build directory before doing the build step shown above:

    > cmake --build build --target clean

## To compile the documentation

Should you need to recompile the Doxygen auto-generated code documentation, run the following command from the evobee base directory:

    > doxygen doxygen.cfg


The generated code documentation can be found in the `docs/html` folder of the evobee base directory.
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE5OTM3NzMzMTMsNjIyMDU4ODEwLDM0Mz
IzMDQ0NiwzNTI5MTkyNjgsLTg2NjY0MTY5MywtMTYzMjQ3NjMx
NiwtMTI5MzUzNTQsLTYxNzUwMzcxNl19
-->