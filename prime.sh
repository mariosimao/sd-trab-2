#!/bin/bash

# Script responsible for generating execution time statistics for the "prime" program

exportFile="prime.csv"

echo "N,Np,Nc,time" >> $exportFile

for n in 1 2 4 8 16 32; do
  for nc in 1 2 4 8; do
    np=1
    echo -n "$n,$np,$nc," >> $exportFile

    totalDuration=0
    for i in {1..10}; do
      duration=$(./prime $n $np $nc)
      totalDuration=$(($totalDuration+$duration))
    done

    avarageDuration=$(($totalDuration/10))
    echo "$avarageDuration" >> $exportFile
  done

  for np in 2 4 8; do
    nc=1
    echo -n "$n,$np,$nc," >> $exportFile

    totalDuration=0
    for i in {1..10}; do
      duration=$(./prime $n $np $nc)
      totalDuration=$(($totalDuration+$duration))
    done

    avarageDuration=$(($totalDuration/10))
    echo "$avarageDuration" >> $exportFile
  done
done
