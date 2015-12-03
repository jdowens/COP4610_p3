#!/bin/bash

make clean
make
for x in {1..5}
do
	echo mkdir $x >> input.txt
	echo cd $x >> input.txt
	for y in {1..10}
	do
		echo create $y.txt >> input.txt
	done
	echo cd .. >> input.txt
done

for x in {6..10}
do
	echo mkdir $x >> input.txt
	echo cd $x >> input.txt
	for y in {1..3}
	do
		echo create $y.txt >> input.txt
	done
done
echo exit >> input.txt
