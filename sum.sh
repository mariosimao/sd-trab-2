#!/bin/bash

# Script responsible for generating execution time statistics for the "sum" program

exportFile="sum.csv"

echo "n,k,time" >> $exportFile

for n in 10000000 100000000 1000000000; do
  for k in 1 2 4 8 16 32 64 128 256; do
    echo -n "$n,$k," >> $exportFile

    totalDuration=0
    for i in {1..10}; do
      duration=$(./sum $n $k)
      totalDuration=$(($totalDuration+$duration))
    done

    avarageDuration=$(($totalDuration/10))
    echo "$avarageDuration" >> $exportFile
  done
done
