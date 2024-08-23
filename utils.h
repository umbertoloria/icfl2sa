#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include<iostream>
#include<string.h>
#include<unordered_map>
#include "generic_vector.h"
#include "custom_suffix_tree_structs.h"
#include <algorithm>
#include "mutex"
#include "radix_sort.h"
#include <execution>
#include <unordered_set>

using namespace std;


void print(string word);
void printVector(vector<int> vec, string msg);
void printVec(vector<int> vec);
void printDict(std::map<int,std::vector<int>>& index_to_nodes);
void printUnorderedSet(std::unordered_set<int>& mySet);
void printBoolVec(vector<bool> vec);
char* append(const char *s, char c);
void quicksort(int_vector* x, int start, int end);
void quicksort_of_nodes(nodes_vector* x, int start, int end);
void quicksort_of_indexes(const char* S,vector<int>& indexes, int start, int end);
void quicksort_of_indexes_2(const char* S,vector<int>& indexes, int start, int end,int start_offset);
void quicksort_of_indexes_3(const char* S,vector<int>& indexes, int start, int end,int start_offset,std::unordered_map<int,std::unordered_map<int,bool>*>& ord);
void quicksort_of_indexes_4(const char* S,vector<int>& indexes);
void quicksort_of_indexes_4_2(const char* S,vector<int>& indexes,int starting_offset);
void quicksort_of_indexes_5(const char* S,vector<int>& indexes);
void quicksort_of_indexes_5_2(const char* S,vector<int>& indexes,int starting_offset);
void quicksort_of_indexes_6(const char* S,vector<int>& indexes);
void quicksort_of_indexes_7(const char* S,vector<int>& indexes,int lenght_of_word);
void quicksort_of_indexes_8(const char* S,vector<int>& indexes);

void print_substring(const char* str,int n);
int fast_compare( const char *ptr0, const char *ptr1, int len );

char* get_substring(const char* basestring,int len);
size_t get_hash_of_subsring(char* str);
unsigned long hash_substring(const char *str,int size);
#endif