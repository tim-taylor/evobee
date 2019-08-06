#!/usr/bin/env python3
#
# Script to plot evobee results from multiple runs with error bars
#
# Usage: plot-results.py title statsfile [statsfile2 [statsfile3 ...]]
#   where each statsfile is a CSV format with layout: SZ,N,min,Q1,median,Q3,max,mean,sd
#    C=constancy, P=patchiness, and the rest is the output of the command:
#    sta --min --max --mean --sd --sample --q --delimiter $',' < pollination-50pc-times-s$S-c0p$C.txt
#
# Outputs: a graphics file called graph-<title>.pdf
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 9 October 2017

import sys
import os
import matplotlib.pyplot as plt

def main():

    # check we have all of the required command line info
    if len(sys.argv) != 2:
        print("Usage: {} datafile"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    #title = sys.argv[1]
    #statsfilenames = [sys.argv[i] for i in range(2, len(sys.argv))]
    datafile = sys.argv[1]

    if not os.path.exists(datafile) or not os.path.isfile(datafile):
        print("Stats file '{}' does not exist or is not a regular file!".format(datafile), file=sys.stderr)
        exit(1)

    with open(datafile, 'r') as f:
        data = [int(x) for x in f.read().splitlines()]

    #print(data)

    plt.violinplot(data, points=99, widths=0.5, showmeans=True, showextrema=True, showmedians=True)
    plt.ylim(0,1000)
    plt.xticks([])

    plt.show()



    # for sfile in statsfilenames:

    #     if not os.path.exists(sfile) or not os.path.isfile(sfile):
    #         print("Stats file '{}' does not exist or is not a regular file!".format(sfile), file=sys.stderr)
    #         exit(1)

    #     with open(sfile, 'r') as f:
    #         reader = csv.reader(f, quoting=csv.QUOTE_NONNUMERIC)
    #         data = list(reader)

    #     x = [row[0] for row in data] # EnvSize
    #     y = [row[4] for row in data] # Median
    #     #y = [row[7] for row in data] # Mean
    #     e = [row[8] for row in data] # StdDev

    #     xmin = x[0]
    #     xmax = x[-1]

    #     m = re.match(r".*-(.*).txt", sfile)
    #     desc = m.group(1)

    #     plt.errorbar(x, y, yerr=e, fmt='-o', label=desc)


    # plt.xlabel('Environment size')
    # plt.ylabel('Fixation time')
    # plt.legend(loc='upper left', prop={'size': 10})
    # plt.title(title)
    # plt.grid()
    # plt.xlim(xmin-2, xmax+2)
    # #plt.ylim(140,350)
    # #plt.ylim(140,550)
    # #plt.show()

    # # Replace spaces etc in title so we can use it in the filename of the graph
    # filename = 'graph-'+title+'.pdf'
    # for ch in [' ',',','(',')','[',']']:
    #     if ch in filename:
    #         filename = filename.replace(ch,"-")
    # filename = filename.replace('---','-')
    # filename = filename.replace('--','-')


    # plt.savefig(filename)



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
