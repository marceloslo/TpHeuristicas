#!/bin/bash
for filename in ./test/*; do 
	f=$(basename "$filename")
	for i in {1..10}; do
		./main.exe 100 50 50 100 "g" < "$filename" >> "./result/$f"
	done
done

