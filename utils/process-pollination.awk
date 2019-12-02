# Script to process the output of an evobee run and output
# aggregate data of the pollen deposited on each species of
# flower (the count of the number of conspecific pollen grains)
#
# Typical usage:
#
# > for F in `ls *log.txt`; do gawk -f process-pollination.awk $F > $F".pollination-counts.csv"; done
# > rm -f pollination-totals.csv 
# > for L in `seq 300 10 650`; do grep "$L," *pollination-counts.csv | gawk -F',' -vL=$L '{totC+=$2; totH+=$3} END {print L "," totC "," totH}' >> pollination-totals.csv; done
#

BEGIN {
    FS=",";
}

$1=="F" && NF>13 {
    mp=$11;
    numpol=(NF-13)/3;
    for (i=0; i<numpol; i++) {
	pf=13+(i*3);
	pmp = $pf;
	pmpc = $(pf+1);
	if (pmp==mp) {
	    conspecific[mp] += pmpc;
	}
	else {
	    heterospecific[mp] += pmpc
	}
    };
}

END {
    for (mp=300; mp<=650; mp+=10) {
	printf "%i,%i,%i\n", mp, conspecific[mp], heterospecific[mp];
    }
}
