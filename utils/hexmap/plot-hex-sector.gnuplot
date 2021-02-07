# Gnuplot script to generate a plot of flower data with flowers laid
# out in hex sector space, for all flower types present in a simulation
# run at the specified generation number, using input data from the
# specified file.
#
# The input file is a 7-field CSV file as generated by the utility script
# gen-hex-plot-data
#
# Call like this, e.g.
# > gnuplot -c plot-hex-sector.gnuplot filebase N
# e.g.
# > gnuplot -c plot-hex-sector.gnuplot hex-info 100
#
# The script looks for an input data file named [filebase]-gen-N.csv
#
# The output is saved in a file named [filebase]-gen-N-hex-sector.png

if (exists("ARG1")) {
    filebase = ARG1
}
else {
    filebase="hex-info"
}

if (exists("ARG2")) {
    gen = ARG2
}
else {
    gen = 0
}

fbase = sprintf("%s-gen-%s", filebase, gen)
infile = sprintf("%s.csv", fbase)
outfile = sprintf("%s-hex-sector.png", fbase)

set datafile separator ","
set xrange [0:360]
set yrange [0:20000]
set xlabel "Hex Sector (degrees)"
set ylabel "Count"
set title "Generation ".gen
set key
PSZ=0.9
set term png
set output outfile
plot infile using 4:5 pt 7 lc rgb "red" ps PSZ title "flowers", infile using 4:6 pt 7 lc rgb "blue" ps PSZ title "pollinations", infile using 4:7 pt 7 lc rgb "green" ps PSZ title "landings"
