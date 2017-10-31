#!/usr/bin/env python3
#
# Script to generate an evobee configuration file based upon a given template and specific parameter overrides
#
# Usage: evobee-config-gen templatefile outputfile param1=param1val [param2=param2val [param3=param3val ...]]
#
#  Note: for cases where a parameter name is used in multiple positions in the config file, you can use
#        the syntax sct:param=val to specify the specific section of the Json file in which the desired
#        parameter is located. In any case, the script only ever changes one instance of each parameter.
#
# Outputs: a new config file named <outputfile> based upon <templatefile> with the specified amendments
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 31 October 2017

import sys
import os
import re

def main():

    # check we have all of the required command line info
    if len(sys.argv) < 4:
        print("Usage: {} templatefile outputfile param1=param1val [param2=param2val [param3=param3val ...]]"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    templatefilename = sys.argv[1]
    outputfilename = sys.argv[2]
    params = {}
    while len(sys.argv) > 3:
        ppair = sys.argv[3].split("=",1)
        if len(ppair) != 2:
            print("Unrecognised parameter specification '{}', expecting format 'param=val'".format(sys.argv[3]), file=sys.stderr)
            exit(1)
        params[ppair[0]] = ppair[1]
        del sys.argv[3]

    #print("Params are: {}".format(params))

    # Check that the specified template file does exit
    if not os.path.exists(templatefilename) or not os.path.isfile(templatefilename):
        print("Template file '{}' does not exist or is not a regular file!".format(templatefilename), file=sys.stderr)
        exit(1)

    # Check that the specified output file does not already exist
    if os.path.exists(outputfilename):
        print("Output file '{}' already exists!".format(outputfilename), file=sys.stderr)
        exit(1)

    # Read the content of the template file into a variable
    with open (templatefilename, 'r' ) as f:
        content = f.read()

    # For each of the parameters specidied on the command line
    for param, val in params.items():
        # Swap the current entry in the file with the specified value

        pspec = param.split(":",1)
        if len(pspec) == 2:
            # this param belongs to a specific section of the Json file, so we must look in the right place
            sct = pspec[0]
            prm = pspec[1]
            reg = '("' + sct + '"\s*:\s*\{[^\}]*"' + prm + '"\s*:\s*"?)([A-Za-z0-9.\-/]*)("?)'
        else:
            # no section specified, so we can just do a simpler search for the parameter name by itself
            reg = '("' + param + '"\s*:\s*"?)([A-Za-z0-9.\-/]*)("?)'

        rep = r'\g<1>' + val + r'\g<3>'
        content = re.sub(reg, rep, content, flags = re.M)

    # Write the new content to the output file
    with open (outputfilename, 'w') as f:
        print(content, file=f)


##-------------------------------------------------------##

if __name__ == '__main__':
    main()
