#include "../headers/helpers.h"

/**
  *   @file helpers.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file helpers.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 19-10-2017
  **/

int linear_search(void* table, int num_elems, size_t size, void* key, int (*comp) (const void* arg1, const void* arg2)){
    int i;
    for (i = 0;i<num_elems;i++){
        if ((*comp)(key, table+i*size) == 0){
            return i;
        }
    }
    return -1;
}
int binary_search(void* table, int first, int last, size_t size, void* key, int (*comp) (const void* arg1, const void* arg2)){

	int med;
    if (last < first){
        return -1;
    } else {
        med = (first + last) / 2;
        if ((*comp)(key, table+med*size) > 0){
            return binary_search(table, med+1, last, size, key, comp);
        } else if ((*comp) (key, table+med*size) < 0){   /*Recursion*/
            return binary_search(table, first, med-1, size, key, comp);
        } else {
            return med;
        }
    }
    return -1;
}


void insert_sort(void* table, size_t size, int nmemb, void* elem, void* key, int (*comp) (const void* arg1, const void* arg2), void (*insert)(void* table, void* key, int pos)){
	int i;
	if (nmemb == 1){
        (*insert) (table, elem, 0);
    } else {
        i = nmemb-2;
        while(i>=0 && (*comp)(key, table+i*size) < 0) {
        	(*insert) (table, table+i*size, i+1);
            i--;
        }
        (*insert) (table, elem, i+1);
    }
}