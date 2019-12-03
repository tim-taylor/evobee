# Script to process the output of an evobee run and output
# aggregate data of the pollen deposited on each species of
# flower (the count of the number of conspecific pollen grains)
#
# Typical usage: (see also comments in process-pollen-distrib.awk file)
#
# > for F in `ls *log.txt`; do gawk -f process-pollination.awk $F > $F".pollination-counts.csv"; done
# > for F in `ls *log.txt`; do gawk -f ../../../utils/process-pollination.awk $F > $F".heterospecific-distrib.csv" ; done
# > for F in `ls *log.txt`; do gawk -f ../../../utils/process-pollination.awk $F > $F".all-pollen-distrib.csv" ; done
#
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
            heterospecific[mp] += pmpc;
        }
        h[mp][pmp] = pmpc;
    };
}

END {
    # this block outputs data in format for pollination-totals.csv
#     for (mp=300; mp<=650; mp+=10) {
#         printf "%i,%i,%i\n", mp, conspecific[mp], heterospecific[mp];
#     }

    # this block outputs data in format for heterospecifc-distrib.csv
#     for (mp=300; mp<=650; mp+=10) {
#         printf "%i", mp
#         for (pmp=300; pmp<=650; pmp+=10) {
#             if (mp==pmp) {
#                 printf ",%i,%i", pmp, 0;
#             }
#             else {
#                 printf ",%i,%i", pmp, h[mp][pmp];
#             }
#         }
#         printf "\n";
#     }
    
    # this block outputs data in format for all-pollen-distrib.csv
    for (mp=300; mp<=650; mp+=10) {
        printf "%i", mp
        for (pmp=300; pmp<=650; pmp+=10) {
            printf ",%i,%i", pmp, h[mp][pmp];
        }
        printf "\n";
    }    
}
