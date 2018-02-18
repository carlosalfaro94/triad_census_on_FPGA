#!/bin/bash

source scripts/init.sh

make times_seq
make times_par

#for n in 400000
#do
#k=10
#m=$(($n*$k))
#exe/times_parallel -g graphs/aux_NDRange_BM_ord.txt -b binaries/tc_NDRange_BM_ord.aocx -f NDRange_BM_ord -t times/NDRange_BM_ord.txt -n $n -e $m -i 2 -o
#exe/times_sequential -g graphs/aux_seq_BM_ord.txt -f BM -t times/seq_BM_ord.txt -n $n -e $m -i 2 -o
#done

rm -f graphs/aux_seq_BM_ord.txt graphs/aux_NDRange_BM_ord.txt
make clean


for n in 400000
do
avg_seq=$(cat times/seq_BM_ord.txt | grep -w $n | awk 'BEGIN {sum = 0;c=0} {sum+=$4;c++} END {ave = sum / c; print ave}')
echo -e "$n\t$avg_seq" >> "times/avg_seq_BM_ord.txt"
avg_NDR=$(cat times/NDRange_BM_ord.txt | grep -w $n | awk 'BEGIN {sum = 0;c=0} {sum+=$4;c++} END {ave = sum / c; print ave}')
echo -e "$n\t$avg_NDR" >> "times/avg_NDRange_BM_ord.txt"
done

#rm -f times/seq_BM_ord.txt times/NDRange_BM_ord.txt
