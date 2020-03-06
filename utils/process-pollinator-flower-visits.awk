# Script to process logging output with Q flag
# (PollinatorIntraPhaseFull) and extract details
# of flower visits and rewards from a single specified
# pollinator (specified by pollinator id as an
# argument to the script).
#
# Each line of the output shows the following fields:
# >  step, pollinatorID, targetMP, flowerMP (or 0)*, reward (or -1,-2)**, judgedToMatchTarget
#
# *  0 if pollinator is not currently landed on a flower
# ** -1 if pollinator saw a flower this step but decided not a land on it
#    -2 if pollinator did not see a flower this step
#
# Sample usage:
# > gawk -vPID=1 -f process-pollinator-flower-visits.awk run-log.txt
#
# To filter the output use the command line option -vL=[n]
# where [n] can be one of the following values:
#  -vL=1 only show entries where a landing occured
#  -vL=2 only show entries where a flower was seen but pollinator decided not to land
#  -vL=3 only show entries where no flower was seen
#
# e.g.
# > gawk -vPID=1 -vL=1 -f process-pollinator-flower-visits.awk run-log.txt
#
# or a more complicated example to separate out results involving a flower judged to
# match the current target from those involving flowers judged to not match the target:
#
# > for T in "T" "F"; do for V in 1 2 3; do echo "---" $T $V "---"; for P in `seq 1 100`; do
# >   gawk -vPID=$P -vL=$V -f process-pollinator-flower-visits.awk run-log.txt;
# >   done | gawk -F',' -vT=$T
# >   'NR>1 && $6==T {if($3>$4){E=($3-$4)}else{E=($4-$3)};if(E<200){N++};if(N>1){D+=E}} END {N=N-1; print N " " D/N}';
# >   done; done
#

BEGIN {
    FS=",";
}

$1=="Q" && $5==PID {
    if (L==0 || (L==1 && $12>=0) || (L==2 && $12==-1) || (L==3 && $12==-2)) {
        printf "%s,%s,%s,%s,%s,%s\n", $3, $5, $15, $11, $12, $13;
    }
}
