#include "graph.h"


#define NUM_TRIADS 16  /*!< Number of distinct possible triads (considering isomorphisms)*/

/**
  * Function: triad_census_BF
  *
  * @brief Function that implements the Brute Force version
  * of the triad census algorithm
  * 
  * @param GRAPH* g: Pointer to the graph of which we want 
  * to perform the triad census
  * 
  *	@return int*: a vector containing the triad counts
  *
  **/
uint64_t* triad_census_BF(GRAPH* g);

/**
  * Function: triad_census_MV
  *
  * @brief Function that implements the Batagelj and Mrvar's version
  * of the triad census algorithm
  * 
  * @param GRAPH* g: Pointer to the graph of which we want 
  * to perform the triad census
  * 
  *	@return int*: a vector containing the triad counts
  *
  **/
uint64_t* triad_census_BM(GRAPH* g);

/**
  * Function: print_triad_census
  *
  * @brief Function that prints the triad census
  * 
  * @param int* census: The vector containing the triad census  
  * 
  *	@return None
  *
  **/
void print_triad_census(uint64_t* triads);

//void print_triad_census_64b(uint32_t* triads);

/**
  * Function: return_code
  *
  * @brief Function returns the code of a triad (for testing purposes)
  * 
  * @param int* census: The vector containing the triad census  
  * 
  *	@return const char*: The triad code
  *
  **/
const char* return_code(int* census);

/**
  * Function: isoTricode
  * @brief Function that computes the triad type of a triple of nodes
  * 
  * @param NODE* u: First node of the triple
  * @param NODE* v: Second node of the triple
  * @param NODE* w: Third node of the triple
  * 
  *	@return int: A number between 0 and 15, depending on the configuration 
  * of the triple
  *
  **/
int isoTricode(NODE* u, NODE* v, NODE* w);

#if 0
int isoTricode(NODE* u, NODE* v, NODE* w);


int check_diad(NODE* a, uint32_t bid);


int check_two_diads(NODE* a, uint32_t bid, uint32_t cid);


int check_three_diads(NODE* a, NODE* b, uint32_t bid, uint32_t cid);
#endif

/**
  * Function: num_total_triads
  * @brief Function that computes and returns the number of triads 
  * present in a graph of n nodes
  * 
  * @param uint32_t n: The number of nodes of a graph 
  * 
  *	@return uint32_t: The number of total triads
  *
  **/
uint64_t num_total_triads (uint32_t n);