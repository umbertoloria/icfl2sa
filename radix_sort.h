#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <map>
#include <unordered_map>
#include <cstdint>

void radixsort(const char** arr, int n);
void radixsort_msd(const char** arr, int n,const char* S, int lenght_of_word);
