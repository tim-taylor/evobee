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

<!--stackedit_data:
eyJoaXN0b3J5IjpbMzUyOTE5MjY4LC04NjY2NDE2OTMsLTE2Mz
I0NzYzMTYsLTEyOTM1MzU0LC02MTc1MDM3MTZdfQ==
-->