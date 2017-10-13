#!/usr/bin/env python3
#
# Script to plot evobee results from multiple runs with error bars
#
# Usage: plot-results.py title statsfile [statsfile2 [statsfile3 ...]]
#   where each statsfile is a CSV format with layout: C,P,N,min,Q1,median,Q3,max,mean,sd
#    C=constancy, P=patchiness, and the rest is the output of the command:
#    sta --min --max --mean --sd --sample --q --delimiter $',' < pollination-50pc-times-s$S-c0p$C.txt
#
# Outputs: a graphics file called graph-<title>.pdf
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 9 October 2017

import sys
import os
import csv
import matplotlib.pyplot as plt


def main():

    # check we have all of the required command line info
    if len(sys.argv) < 3:
        print("Usage: {} title statsfile [statsfile2 [statsfile3 ...]]"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    title = sys.argv[1]
    statsfilenames = [sys.argv[i] for i in range(2, len(sys.argv))]

    for sfile in statsfilenames:

        if not os.path.exists(sfile) or not os.path.isfile(sfile):
            print("Stats file '{}' does not exist or is not a regular file!".format(sfile), file=sys.stderr)
            exit(1)

        with open(sfile, 'r') as f:
            reader = csv.reader(f, quoting=csv.QUOTE_NONNUMERIC)
            data = list(reader)

        x = [row[1] for row in data]
        y = [row[8] for row in data]
        e = [row[9] for row in data]

        xmin = x[0]
        xmax = x[-1]
        desc = 'Constancy 0.'+str(int(data[0][0]))

        plt.errorbar(x, y, yerr=e, fmt='-o', label=desc)


    plt.xlabel('<--big patches         Patchiness         small patches-->')
    plt.ylabel('Time to 50% pollination of Species 1')
    plt.legend(loc='lower right')
    plt.title(title)
    plt.grid()
    plt.xlim(xmin-2, xmax+2)
    #plt.ylim(140,350)
    #plt.show()

    t2 = title
    for ch in [' ',',']:
        if ch in t2:
            t2=t2.replace(ch,"-")

    plt.savefig('graph-'+t2+'.pdf')



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
