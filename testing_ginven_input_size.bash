#!/bin/bash

python3 python_string_generator.py $1;
./main 1
for i in $(seq 1 4);
do
	./main $(($i*2))
done
