#!/usr/bin/env python3
#
# Script to plot chart of the numbers of two plant species over a number of
# generations, using recorded data from a single run
#
# Usage: plot-two-species-over-time.py logfilebasename xmax ymax title
#   The script expects to find two datafiles, each containing data about the
#     evolution of a single plant species over time, named:
#       logfilebasename-s1.dat
#       logfilebasename-s2.dat
#   where each of these files is a csv file with each line in the format:
#       GENNUM, NUMPLANTS
#   The other parameters determine how the histogram is plotted.
#
# Outputs: two versions of the graph are output, one in png format
#   and the other in svg format:
#      species-over-time-plot-<datafilebasename>.png
#      species-over-time-plot-<datafilebasename>.svg
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 24 June 2020

import re
import sys
import os
import matplotlib.pyplot as plt
import numpy as np

def main():

    # check we have all of the required command line info
    if len(sys.argv) != 5:
        print("Usage: {} logfilebasename xmax ymax title"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    logfilebasename = sys.argv[1]
    xmax = int(sys.argv[2])
    ymax = int(sys.argv[3])
    title = sys.argv[4]

    datafile1 = logfilebasename + "-s1.dat"
    datafile2 = logfilebasename + "-s2.dat"

    if not os.path.exists(datafile1) or not os.path.isfile(datafile1):
        print("Stats file '{}' does not exist or is not a regular file!".format(datafile1), file=sys.stderr)
        exit(1)

    if not os.path.exists(datafile2) or not os.path.isfile(datafile2):
        print("Stats file '{}' does not exist or is not a regular file!".format(datafile2), file=sys.stderr)
        exit(1)

    # try to automatically extract a run id number from the input data filename
    try:
        m = re.match(r".*-(.*)-log\.*", logfilebasename)
        runid = m.group(1)
        plottitle = "Sample run [id {}]\n{}".format(runid, title)
    except IndexError:
        plottitle = title

    outnamepng = "species-over-time-plot-"+os.path.basename(datafile1)+".png"
    outnamesvg = "species-over-time-plot-"+os.path.basename(datafile1)+".svg"

    (x1faw,y1faw) = np.genfromtxt(datafile1, delimiter=',', unpack=True)
    (x2faw,y2faw) = np.genfromtxt(datafile2, delimiter=',', unpack=True)
    plt.plot(x1faw, y1faw, label="Species X")
    plt.plot(x2faw, y2faw, label="Species Y")
    plt.axis([0, xmax, 0, ymax])
    plt.legend(loc="lower right")  # options for loc include "best", "center right", "lower right", etc
    plt.xlabel('Generation')
    plt.ylabel('Number of Plants')
    plt.title(plottitle)

    plt.savefig(outnamepng)
    plt.savefig(outnamesvg)




    # #plt.figure().set_size_inches(7.91, 2.88)
    # #plt.figure().set_size_inches(3.95, 2.88)
    # plt.ylim(top=ymax)
    # plt.xticks(np.arange(tickspace,xmax,tickspace))
    # plt.hist(fixtimes, bins=np.linspace(xmin+0.5,xmax-0.5,numbins),  rwidth=0.8)
    # plt.grid()
    # plt.axis([xmin, xmax, 0, ymax])
    # plt.xlabel("Fixation time (generations)")
    # plt.ylabel("Frequency")
    # plt.title(title)
    # #plt.title(title, fontsize=10)

    # #plt.tight_layout()



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
