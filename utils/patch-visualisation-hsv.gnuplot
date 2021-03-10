# Gnuplot script to generate a visualisation of the distribution of flowers in the environment at a
# given point in time, using log data generated using the "F" flag (logFlowersInterPhaseFull).
#
# call this with, e.g.
# > gnuplot -e "gen=0;filebase='flower-positions';envsize=240" patch-visualisation-hsv.gnuplot
# or
# > for G in `seq 0 10 90`; do gnuplot -e "gen=$G;filebase='flower-positions'" patch-visualisation-hsv.gnuplot; done
#
# with a specified filename base "filebase" and a generation number "gen", the script expects to find
# an input file called "filebase"-g"gen".csv
#
# The output is saved to a file called "filebase"-g"gen".png
#
# The input files should have lines of the following format:
#    x pos, y pos, speciesID (id as used in JSON file)
#
# These can be generated from the raw log files as follows:
# > for G in `seq 0 10 99`; do gawk -F',' -vG=$G '$1=="F" {if ($2==G) {print $5 "," $6 "," $4}}' run-log.txt > flower-positions-g$G.csv; done
#
# Info on using HSV in Gnuplot:
# https://stackoverflow.com/questions/35560805/gnuplot-line-color-in-hsv-hue-saturation-value-model
#

# The following procedure will generate the expected input files, which have lines in the format:
#
#   1      2      3       4       5      6      7      8           9
#   ENV_X, ENV_Y, RUN_ID, AUX_ID, HEX_X, HEX_Y, THETA, DOM_LAMBDA, DIST_FROM_ORIGIN
#
#
# Note that this procedure requires the log file to contain data from both flags "F" and "n".
#
# MFILE=/home/tim/evobee-hexmap/aus-data-20210111/aus-hex-space-info.csv
# LOGFILE=aus-dataset-1-only-20210223-sample-1-2021-03-08-16-23-14-496119-log.txt
# GENFIRST=0
# GENLAST=99
# GENSTEP=1
#
# gawk -F, '$1=="n" && $2==0 {print $4","$9}' $LOGFILE > runid-to-auxid-map.csv
#
# for G in `seq $GENFIRST $GENSTEP $GENLAST`; do gawk -F, -vMFILE=$MFILE -vG=$G '$1=="F" && $2==G {RID=$4; ENVX=$5; ENVY=$6; cmd1="grep \"^"RID",\" runid-to-auxid-map.csv"; cmd1 | getline line; close(cmd1); comidx=index(line, ","); AID=substr(line, comidx+1); cmd2="grep \"^"AID",\" "MFILE; cmd2 | getline info; close(cmd2); printf("%.5f, %.5f, %d, %s\n", ENVX, ENVY, RID, info);}' $LOGFILE > flower-positions-g$G-extra.csv; done


if (!exists("gen")) gen=0
if (!exists("filebase")) filebase="flower-positions"
if (!exists("envsize")) envsize=300

mytitle = sprintf("Flower distribution at generation %d", gen)
datafile = sprintf("%s-g%d-extra.csv", filebase, gen)
outfile = sprintf("%s-g%04d-hsv.png", filebase, gen)

set terminal png size 800,800
set output outfile

set size square
set datafile separator ","
set xrange [0:envsize]
set yrange [envsize:0]
set title mytitle
unset key
set xtics 20
set ytics 20

#plot datafile using 1:2:3 with points pt 7 ps 0.5 lc variable
plot datafile using 1:2:(hsv2rgb($7/360.0,0.3+$9/0.7,1)) with points pt 7 ps 0.5 lc rgbcolor variable

