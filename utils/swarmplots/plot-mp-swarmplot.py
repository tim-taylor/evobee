#!/usr/bin/env python3
#
# Script to generate a violin plot of the distribution of flower marker points
# from a collection of runs of evobee
#
# Usage: plot-mp-distrib.py title mpcountmpfile [mpcountmpfile2 [mpcountmpfile3 ...]]
#   where each mpcountmpfile is a CSV format with layout: marker point, count
#
# Outputs: a graphics file called mp-distrib-violins-<title>.pdf
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 25 May 2020

import sys
import os
import csv

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def main():

    # check we have all of the required command line info
    if len(sys.argv) < 3:
        print("Usage: {} title mpcountmpfile [mpcountmpfile2 [mpcountmpfile3 ...]]"
            .format(os.path.basename(sys.argv[0])), file=sys.stderr)
        sys.exit(1)

    # parse the command line info
    title = sys.argv[1]
    mpfilenames = [sys.argv[i] for i in range(2, len(sys.argv))]

    #df = pd.DataFrame(index=[], columns=[0,1])

    # initialise dataframe with a dummy entry of -1000 for each of the marker points of
    # interest. This ensures the plot shows the full range of marker points even if some
    # don't have any data points. We restrict the display of the y axis to values of
    # 0 or higher so we don't see these dummy entries in the plot.
    df = pd.DataFrame({ 0: range(380, 581, 10), 1: [-1000 for i in range(380, 581, 10)]})

    for mpfile in mpfilenames:

        if not os.path.exists(mpfile) or not os.path.isfile(mpfile):
            print("Stats file '{}' does not exist or is not a regular file!".format(mpfile), file=sys.stderr)
            exit(1)
            
        if os.path.getsize(mpfile) > 0:
            dftmp = pd.read_csv(mpfile, header=None)
            df = df.append(dftmp, ignore_index=True, sort=False)

    df = df.rename(columns={0: "marker_point", 1: "count"})

    #sns.set(style="whitegrid", palette="muted")
    plt.xlim(380,580)
    #plt.ylim(0,5500)
    plt.ylim(0,23500)    
    plt.xticks(rotation=90)
    plt.title(title)


    #sns.violinplot(x="marker_point", y="count", data=df, inner=None)
    #sns.swarmplot(x="marker_point", y="count", data=df, color="white", edgecolor="gray")

    sns.swarmplot(x="marker_point", y="count", data=df)

    #plt.show()


    # plt.xlabel('Environment size')
    # plt.ylabel('Fixation time')
    # plt.legend(loc='upper left', prop={'size': 10})
    # plt.title(title)
    # plt.grid()
    # plt.xlim(xmin-2, xmax+2)
    # #plt.ylim(140,350)
    # #plt.ylim(140,550)
    # #plt.show()

    # Replace spaces etc in title so we can use it in the filename of the graph
    filename = 'mp-distrib-swarmplot-'+title+'.pdf'
    for ch in [' ',',','(',')','[',']']:
        if ch in filename:
            filename = filename.replace(ch,"-")
    filename = filename.replace('---','-')
    filename = filename.replace('--','-')


    plt.savefig(filename)



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
