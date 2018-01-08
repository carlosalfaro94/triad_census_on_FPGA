#include <stdlib.h>   /*Useful for size_t*/


int linear_search(void* table, int num_elems, size_t size, void* key, int (*comp) (const void* arg1, const void* arg2));
/**
  * Function: binary_search
  * @brief Function that performs binary search over an ordered array 
  * 
  * @param void* table: The array in which to perform the search
  *	@param int first: the first index of the table
  * @param int last: the last index of the table
  * @param size_t size: size in bytes of each of the elements of the array
  * @param void* key: The key to search within the array
  * @param int (*comp) (const void* arg1, const void* arg2): pointer to the campare function
  * 
  *	@return int the position in which the key is stored, if it is found
  *			-1 otherwise
  *
  **/
int binary_search(void* table, int first, int last, size_t size, void* key, int (*comp) (const void* arg1, const void* arg2));


/**
  * Function: insert_sort
  * @brief Function that performs the insertion sort (inner loop of the algorithm)
  * 
  * @param void* table: The array in which to perform the insertion
  * @param size_t size: size in bytes of each of the elements of the array
  * @param int nmemb: Number of elements present in the array
  * @param void* elem: element to insert
  * @param void *key: key associated to the element, useful to compare to other elements
  * @param int (*comp) (const void* arg1, const void* arg2): pointer to the compare function
  * void (*insert)(void* table, void* key, int pos):pointer to the insertion function
  *
  *	@return int the position in which the key is stored, if it is found
  *			-1 otherwise
  *
  **/
void insert_sort(void* table, size_t size, int nmemb, void* elem, void* key, int (*comp) (const void* arg1, const void* arg2), void (*insert)(void* table, void* key, int pos));