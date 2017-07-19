#!/usr/bin/python3
#
# Script to generate a .cpp and .h file for a new class
#
# Usage: newclass NewClassName
#
# Outputs: NewClassName.cpp and NewClassName.h
#
# Checks if .cpp or .h file already exist. If so, complains
# and exits without attempting to create the files.
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 3 May 2017

import sys
import os

def main():
    if len(sys.argv) != 2:
        print("Usage: {} classname".format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    className = sys.argv[1]
    cppFilename = className + ".cpp"
    hFilename = className + ".h"

    if os.path.exists(cppFilename) or os.path.exists(hFilename):
        print("Requested output files already exist! Aborting", file=sys.stderr)
        sys.exit(1)

    try:
        cppFile = open(cppFilename, 'w')
        print("""/**
 * @file
 *
 * Implementation of the {cn} class
 */

#include \"{hfn}\"

{cn}::{cn}() {{
}}""".format(cfn=cppFilename,
            hfn=hFilename,
            cn=className), file=cppFile)
    except IOError as e:
        print("Unable to open file {} for writing: {}".format(cppFilename, str(e), file=sys.stderr))
        sys.exit(1)
    finally:
        cppFile.close()

    try:
        hFile = open(hFilename, 'w')
        print("""/**
 * @file
 *
 * Declaration of the {cn} class
 */

#ifndef {ig}
#define {ig}

/**
 * The {cn} class ...
 */
class {cn} {{

public:
    {cn}();
    ~{cn}() {{}}
}};

#endif /* {ig} */""".format(hfn=hFilename,
                            ig="_"+hFilename.upper().replace(".","_"),
                            cn=className), file=hFile)
    except IOError as e:
        print("Unable to open file {} for writing: {}".format(hFilename, str(e), file=sys.stderr))
        sys.exit(1)
    finally:
        hFile.close()


##-------------------------------------------------------##

if __name__ == '__main__':
    main()
