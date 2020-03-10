# Script to generate a configuration file based upon a
# template given as input. The input file is expected to
# have 36 refuge areas for each of the marker points in
# the range 300:10:650.  This script generates a new
# configuration file where the order of the refuges is
# shuffled randomly.
#
# Typical usage:
#
# > gawk -f gen-shuffled-refuges.awk evobee.cfg.json

BEGIN {
    i=1;
    N=1;
    cmd="shuf -e `seq 300 10 650`";
    while ( (cmd | getline x) > 0) {
	numbers[i]=x;
	i++;
    }
    close(cmd);
}

/PlantTypeMP/ && N<=36 {
    sub(/PlantTypeMP[0-9]+/,"PlantTypeMP"numbers[N],$3);
    N++;
}

{
    print;
}
