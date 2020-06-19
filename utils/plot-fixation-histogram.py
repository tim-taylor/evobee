#!/usr/bin/env python3
#
# Script to plot histogram of fixation times from a number of runs
#
# Usage: plot-fixation-histogram.py datafile xmin xmax ymax title
#   where each datafile is a text file with one line per run, where each line
#   contains a single number giving the fixation time for a single run
#   The other parameters determine how the histogram is plotted.
#
# Outputs: a graphics file called fixation-histogram-<datafilebasename>.png
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 19 June 2020

import sys
import os
import matplotlib.pyplot as plt
import numpy as np

def main():

    # check we have all of the required command line info
    if len(sys.argv) != 6:
        print("Usage: {} datafile xmin xmax ymax title"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    datafile = sys.argv[1]
    xmin = int(sys.argv[2])
    xmax = int(sys.argv[3])
    ymax = int(sys.argv[4])
    title = sys.argv[5]

    if not os.path.exists(datafile) or not os.path.isfile(datafile):
        print("Stats file '{}' does not exist or is not a regular file!".format(datafile), file=sys.stderr)
        exit(1)

    fixtimes = np.loadtxt(datafile, comments="NOFIX")

    outname = "fixation-histogram-"+os.path.basename(datafile)+".png"

    span = (xmax-xmin)
    if span <= 25:
        numbins = span
        tickspace = 5
    elif span <= 75:
        numbins = span
        tickspace = 10
    else:
        numbins = 50
        tickspace = 200

    plt.clf()

    #plt.figure().set_size_inches(7.91, 2.88)
    #plt.figure().set_size_inches(3.95, 2.88)

    plt.ylim(top=ymax)
    plt.xticks(np.arange(tickspace,xmax,tickspace))
    plt.hist(fixtimes, bins=np.linspace(xmin+0.5,xmax-0.5,numbins),  rwidth=0.8);
    plt.grid()
    plt.axis([xmin, xmax, 0, ymax])
    plt.xlabel("Fixation time (generations)")
    plt.ylabel("Frequency")
    plt.title(title)
    #plt.title(title, fontsize=10)

    #plt.tight_layout()
    plt.savefig(outname)


##-------------------------------------------------------##

if __name__ == '__main__':
    main()
