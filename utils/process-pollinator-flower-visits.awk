# Script to process logging output with Q flag
# (PollinatorIntraPhaseFull) and extract details
# of flower visits and rewards from a single specified
# pollinator (specified by pollintaor id as an
# argument to the script).
#
# Sample usage:
# > gawk -vPID=1 -f process-pollinator-flower-visits.awk run-log.txt
#
# To only show entries where a landing occured, specify
# command line option -vL=1
# e.g.
# > gawk -vPID=1 -vL=1 -f process-pollinator-flower-visits.awk run-log.txt


BEGIN {
    FS=",";
}

$1=="Q" && $5==PID {
    if (L==0 || (L==1 && $12>=0)) {
	printf "%s,%s,%s,%s,%s\n", $3, $5, $14, $11, $12;
    }
}
