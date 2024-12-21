#!/usr/bin/env bash
g++ -std=c++17 -fopenmp -lpthread -O3 *.cpp -o main
rm main.o
