#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include<iostream>
#include<string.h>
#include<unordered_map>
#include "generic_vector.h"
#include "custom_suffix_tree_structs.h"
#include "mutex"

using namespace std;


void print(string word);
void printVector(vector<int> vec, string msg);
void printVec(vector<int> vec);
char* append(const char *s, char c);
void quicksort(int_vector* x, int start, int end);
void quicksort_of_nodes(nodes_vector* x, int start, int end);
void quicksort_of_indexes(const char* S,vector<int>& indexes, int start, int end);
void print_substring(const char* str,int n);
int fast_compare( const char *ptr0, const char *ptr1, int len );

char* get_substring(const char* basestring,int len);
size_t get_hash_of_subsring(char* str);
unsigned long hash_substring(const char *str,int size);
#endif