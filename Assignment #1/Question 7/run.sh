#!/bin/sh

a=1
while [ "$a" -lt 10 ]
do
  gcc -pthread -o q7 q7.c
  ./q7 "$a"
  a=`expr $a + 1`
done
