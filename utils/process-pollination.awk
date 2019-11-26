# Script to process the output of an evobee run and output
# aggregate data of the pollen deposited on each species of
# flower (the count of the number of conspecific pollen grains)
#
# Typical usage:
#
# > for F in `ls *log.txt`; do gawk -f process-pollination.awk $F > $F".conspecific-counts.csv"; done
# > rm -f conspecific-totals.csv 
# > for L in `seq 300 10 650`; do grep "$L," *conspecific-counts.csv | gawk -F',' -vL=$L '{tot+=$2} END {print L "," tot}' >> conspecific-totals.csv; done
#

BEGIN {
    FS=",";
}

$1=="F" && NF>12 {
    mp=$10;
    numpol=(NF-12)/2;
    for (i=0; i<numpol; i++) {
	pf=12+(i*2);
	pmp = $pf;
	pmpc = $(pf+1);
	if (pmp==mp) {
	    conspecific[mp] += pmpc;
	}
	else {
	    heterspecific[mp] += pmpc
	}
    };
}

END {
#    for (mp in conspecific) {
    for (mp=300; mp<=650; mp+=10) {
	printf "%i,%i\n", mp, conspecific[mp];
    }
}
