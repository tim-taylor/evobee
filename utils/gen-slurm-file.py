#!/usr/bin/env python3
#
# Script to generate a SLURM file for running batches of runs on the MonARCH cluster
#
# Usage: gen-slurm-file basename numruns
#
# Outputs: a new SLURM file called slurm-<runbasename>
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 4 October 2017

import sys
import os

def main():

    # check we have all of the required command line info
    if len(sys.argv) != 3:
        print("Usage: {} basename numruns".format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    basename = sys.argv[1]
    numruns = sys.argv[2]
    slurmfilename = "slurm-" + basename

    # Check that the specified output file does not already exist
    if os.path.exists(slurmfilename):
        print("Output file '{}' already exists!".format(slurmfilename), file=sys.stderr)
        exit(1)

    # Write the new content to the output file
    with open (slurmfilename, 'w') as f:
        print('''#!/bin/env bash
#SBATCH --job-name={bname}
#SBATCH --time=10:00:00
#SBATCH --mem=4000
#SBATCH --array=1-{nruns}
#SBATCH --output={bname}.out

EB_BUILD_DIR=/mnt/lustre/projects/ft13/ttay0006/evobee/code/evobee/build
EB_EXEC=$EB_BUILD_DIR/evobee

$EB_EXEC -c {bname}.cfg.json'''.format(bname=basename, nruns=numruns), file=f)


##-------------------------------------------------------##

if __name__ == '__main__':
    main()
