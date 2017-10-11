#!/usr/bin/env python3
#
# Script to explore relationship between ambient temperature and flower temperature response
#
# Usage:
#
# Outputs:
#
# Author: Tim Taylor (http://www.tim-taylor.com)
# Last update: 9 October 2017

import sys
import matplotlib.pyplot as plt
import numpy as np


def main():

    ambTemps   = {d : 5 + int(30 * np.sin(np.pi * ((d-6)/(20-6)))) for d in range(6,21)}
    #flwrDeltas = {t : (20-t)*0.25 for t in range(0,41)} # linear decreasing
    #flwrDeltas = {t : (t-20)*0.25 for t in range(0,41)} # linear increasing
    flwrDeltas = {t : 3.0*(np.sin(np.pi*2.0*(t+1/40.0))) +
                      3.0*(np.sin(np.pi*4.0*((t+2)/40.0))) +
                      3.0*(np.sin(np.pi*7.0*((t+5)/40.0)))
                      for t in range(0,41)} # all over the shop
    flwrTemps  = {d : t+flwrDeltas[t] for d,t in ambTemps.items()}


    aATt = np.array(list(ambTemps.values()))
    aFTd = np.array(list(flwrTemps.keys()))
    aFTt = np.array(list(flwrTemps.values()))
    maFTtY = np.ma.masked_where(np.logical_and(aFTt < 25, aFTt > aATt+2), aFTt) # red
    maFTtN = np.ma.masked_where(aFTt >= 30, aFTt) # green

    plt.rc('lines', linewidth=1.5)

    plt.subplot(2, 1, 1)
    plt.plot(list(ambTemps.keys()), list(ambTemps.values()), label="Ambient temp")

    plt.plot(list(flwrTemps.keys()), list(flwrTemps.values()), label="Flower temps")
    #fig, ax = plt.subplots()
    #plt.rc('lines', linewidth=1.5)
    #plt.plot(aFTd, maFTtY, aFTd, maFTtN)#, label="Flower temps")

    plt.legend(loc='lower center')
    plt.ylim(0,40)
    plt.xlabel("time of day")
    plt.ylabel("temp")

    plt.subplot(2, 1, 2)
    plt.plot(list(flwrDeltas.keys()), list(flwrDeltas.values()))
    plt.xlabel("ambient temp")
    plt.ylabel("flower delta temp")

    plt.savefig('graph.pdf')



##-------------------------------------------------------##

if __name__ == '__main__':
    main()
