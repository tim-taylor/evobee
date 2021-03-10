# Script to process a run logfile and generate data suitable for
# use by the patch-visualisation-hsv.gnuplot script
#
# The script must be passed the variables (with example values shown):
#
# MFILE=/home/tim/evobee-hexmap/aus-data-20210111/aus-hex-space-info.csv
# R2AFILE=(csv file giving mappings from RunID to AuxID numbers)
# G=(generation number)
#
# Example real-world usage:
#
# LOGFILE=aus-dataset-1-only-20210223-sample-1-2021-03-08-16-23-14-496119-log.txt
# MFILE=/home/tim/evobee-hexmap/aus-data-20210111/aus-hex-space-info.csv
# R2AFILE=runid-to-auxid-map.csv
# GENFIRST=0
# GENLAST=99
# GENSTEP=1
#
# gawk -F, '$1=="n" && $2==0 {print $4","$9}' $LOGFILE > $R2AFILE
#
# for G in `seq $GENFIRST $GENSTEP $GENLAST`; do gawk -vMFILE=$MFILE -vR2AFILE=$R2AFILE -vG=$G -f gen-data-for-hsv-plots.awk $LOGFILE > flower-positions-hsv-gen-$G.csv; done
#

BEGIN {
    FS=","
}

$1=="F" && $2==G {
    RID=$4
    ENVX=$5
    ENVY=$6
    cmd1="grep \"^"RID",\" "R2AFILE
    cmd1 | getline line
    close(cmd1)
    comidx=index(line, ",")
    AID=substr(line, comidx+1)
    cmd2="grep \"^"AID",\" "MFILE
    cmd2 | getline info
    close(cmd2)
    printf("%.5f, %.5f, %d, %s\n", ENVX, ENVY, RID, info)
}
