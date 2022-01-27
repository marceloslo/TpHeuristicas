#!/bin/bash
for filename in ./test/*; do 
	f=$(basename "$filename")
	for i in {1..10}; do
		./main.exe 100 50 30 100 "g" < "$filename" >> "./result/$f"
	done
done

./main.exe 200 100 50 1000 "g" < "./test/scp41.txt" >> "./result/scp41c.txt"
./main.exe 200 100 50 1000 "g" < "./test/scp51.txt" >> "./result/scp51c.txt"
./main.exe 200 100 50 1000 "g" < "./test/scp61.txt" >> "./result/scp61c.txt"