#!/bin/bash

for i in $(find src -print)
do
	echo "File: " $i
	echo "_________________________________"
	cat $i
done
