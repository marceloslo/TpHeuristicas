#!/bin/bash
for filename in ./test/*; do 
	f=$(basename "$filename")
	for i in {1..100}; do
		./main.exe "t" < "$filename" >> "./result/$f"
	done
done