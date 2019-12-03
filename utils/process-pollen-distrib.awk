# Script to process the output of an evobee run and output
# aggregate data of the distribution of pollen deposited
# on each species of flower
#
# Typical usage:
#
# > for F in `ls *log.txt`; do gawk -f ../../../utils/process-pollination.awk $F > $F".all-pollen-distrib.csv" ; done
# > for L in `seq 300 10 650`; do egrep -h "^$L," *all-pollen-distrib.csv | gawk -f ../../../utils/process-pollen-distrib.awk; done > all-pollen-distrib-totals.csv
# > for L in `seq 300 10 650`; do egrep -h "^$L" all-pollen-distrib-totals.csv | gawk -F',' '{for (i=2;i<=NF;i++) {printf "%i", $i; if (i%2==0) {printf ",";} else {printf "\n";}}}' > all-pollen-distrib-totals-$L.csv; done
#

BEGIN {
    FS=",";
}

{
    mp=$1;
    for (i=0; i<36; i++) {
        idx=(i*2)+2;
        pmp=$idx;
        pmpc=$(idx+1);
        num[mp][pmp]+=pmpc;
    }
}

END {
    printf "%i", mp;
    for (pmp=300;pmp<=650;pmp+=10) {
        printf ",%i,%i", pmp, num[mp][pmp];
    }
    printf "\n";
}
