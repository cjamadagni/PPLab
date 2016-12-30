#!/bin/sh

a=1
while [ "$a" -lt 10 ]
do
  gcc -pthread -o q4 q4.c
  ./q4 "$a"
  a=`expr $a + 1`
done
