#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include<iostream>
#include<vector>

#include "generic_vector.h"
#include "custom_suffix_tree.h"

using namespace std;

void print(string word);
void printVector(vector<int> vec, string msg);
void printVec(vector<int> vec);
char* append(const char *s, char c);
void quicksort(int_vector* x, int start, int end);
void quicksort_of_nodes(nodes_vector* x, int start, int end);
void print_substring(const char* str,int n);

char* get_substring(const char* basestring,int len);
size_t get_hash_of_subsring(char* str);
unsigned long hash_substring(const char *str,int size);
unsigned long last_substring_in_map(const char *suffix,int suffix_len,std::map<size_t,std::vector<suffix_tree_node*>>& m);
#endif