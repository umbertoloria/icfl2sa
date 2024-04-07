#!/bin/bash
python3 python_string_generator.py $1
valgrind --tool=massif --stacks=yes --massif-out-file=out_massif_$1_chars ./main
