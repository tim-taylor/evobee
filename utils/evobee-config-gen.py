#!/usr/bin/python3
#
# Script to generate an evobee configure file based upon a given template and specific parameter overrides
#
# Usage: evobee-config-gen templatefile outputfile param1=param1val [param2=param2val [param3=param3val ...]]
#
# Outputs: a new config file named <outputfile> based upon <templatefile> with the specified amendments
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 3 October 2017

import sys
import os

def main():
    if len(sys.argv) < 4:
        print("Usage: {} templatefile outputfile param1=param1val [param2=param2val [param3=param3val ...]]"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    templatefilename = sys.argv[1]
    outputfilename = sys.argv[2]
    params = {}
    while len(sys.argv) > 3:
        ppair = sys.argv[3].split("=",1)
        if len(ppair) != 2:
            print("No, no, no, no!")
            exit(1)
        print("ppair is {}".format(ppair))
        params[ppair[0]] = ppair[1]
        #params['x'] = sys.argv[3]
        del sys.argv[3]

    print("Params are: {}".format(params))



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
