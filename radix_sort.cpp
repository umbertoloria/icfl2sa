#include "radix_sort.h"

#define ALPHABET_SIZE 30

using namespace std;
 
// A utility function to get maximum value in arr[]
const char* getMax(const char** arr, int n)
{
    const char* mx = arr[0];
    for (int i = 1; i < n; i++)
        if (std::strcmp(arr[i],mx)) mx = arr[i];
    return mx;
}
 
// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort(const char** arr, int n, int exp)
{
    // output array
    const char** output=(const char**)malloc(sizeof(*output)*n);
    int i, count[ALPHABET_SIZE] = { 0 };
 
    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[arr[i][exp] - 'A']++;
 
    // Change count[i] so that count[i] now contains actual
    // position of this digit in output[]
    for (i = 1; i < ALPHABET_SIZE; i++)
        count[i] += count[i - 1];
 
    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[arr[i][exp] - 'A'] - 1] = arr[i];
        count[arr[i][exp] - 'A']--;
    }
 
    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];

    free(output);
}

void countSort_msd(const char** arr, int n, int exp,std::unordered_map<const char*,int>& words_lenght)
{
    //'A' + 1 perché 'A' sarà la nuova lettera più piccola lessicograficamente di tutti
    const char** output=(const char**)malloc(sizeof(*output)*n);
    int i, count[ALPHABET_SIZE] = { 0 };
    
    //cout<<"primo for\n";
    for (i = 0; i < n; i++){
        //cout<<"exp: "<<exp<<"\n";
        //cout<<"words_lenght.at(i): "<<words_lenght[arr[i]]<<"\n";
        if(exp<words_lenght[arr[i]]){
            //cout<<"arr[i][exp]: "<<arr[i][exp]<<"\n";
            count[arr[i][exp] - 'A' + 1]++;
        }
        else count[0]++;
    }
    //cout<<"secondo for\n";
    for (i = 1; i < ALPHABET_SIZE; i++)
        count[i] += count[i - 1];
 
    //cout<<"terzo for\n";
    for (i = n - 1; i >= 0; i--) {
        if(exp<words_lenght[arr[i]]){
            output[count[arr[i][exp] - 'A' + 1] - 1] = arr[i];
            count[arr[i][exp] - 'A' + 1]--;
        }
        else{output[count[0] - 1] = arr[i];count[0]--;}
    }

    //cout<<"quarto for\n";
    for (i = 0; i < n; i++)
        arr[i] = output[i];
    
    free(output);
}
 
// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort(const char** arr, int n)
{
    // Find the maximum number to know number of digits
    const char* m = getMax(arr, n);
    cout<<"m: "<<m<<"\n";
    int word_lenght = std::strlen(m);
 
    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 0; exp < word_lenght; ++exp)
        countSort(arr, n, exp);
}

void radixsort_msd(const char** arr, int n,const char* S, int lenght_of_word){
    std::unordered_map<const char*,int> words_lenght;
    int max_lengt=-1;
    //cout<<"a\n";
    for(int i=0;i<n;++i){
        words_lenght[arr[i]]=lenght_of_word-((std::uintptr_t)arr[i]-(std::uintptr_t)S);
        //cout<<"words_lenght.at(i): "<<words_lenght[arr[i]]<<"\n";
        if(words_lenght[arr[i]]>max_lengt) max_lengt=words_lenght[arr[i]];
    }

    //cout<<"b\n";

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    //cout<<"max_lengt: "<<max_lengt<<"\n";
    for (int exp = max_lengt; exp >= 0; --exp){
        //cout<<"exp: "<<exp<<"\n";
        countSort_msd(arr, n, exp,words_lenght);
    }
}