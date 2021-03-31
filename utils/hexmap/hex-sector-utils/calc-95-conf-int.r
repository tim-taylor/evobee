# script to calculate sample mean and 95% confidence interval
# for a list of numbers in a file. Used for generating data
# suitable for plotting as a histogram with confidence interval bars.
# 
# usage:
#   Rscript calc-95-conf-int.r inputfile binnumber
#
# outputs lines of the form:
#   binnumber, numvalues, mean, 95pcConfIntHalf

args <- commandArgs(trailingOnly = TRUE)
myfile <- args[1]
bin <- args[2]
mydata<-read.csv(myfile,header=F,sep=",")
n <- nrow(mydata)
m <- mean(mydata$V1)
ci95 <- qt(.975,n-1)*sd(mydata$V1)/sqrt(n)
cat(sprintf("%s, %d, %.5f, %.5f\n",bin,n,m,ci95))
