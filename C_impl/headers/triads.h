#include "graph.h"

/**
  *   @file triads.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to implement the secuential versions of the Triad Census algorithms. \n
  *   We have implemented the Brute Force (BF) algorithm and the Batagelj and Mrvar's
  *   (BM) algorithm.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

/**
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
  * @brief Function returns the code of a triad (for testing purposes)
  * 
  * @param int* census: The vector containing the triad census  
  * 
  *	@return const char*: The triad code
  *
  **/
const char* return_code(int* census);

/**
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


/**
  * @brief Function that computes and returns the number of triads 
  * present in a graph of n nodes
  * 
  * @param uint32_t n: The number of nodes of a graph 
  * 
  *	@return uint32_t: The number of total triads
  *
  **/
uint64_t num_total_triads (uint32_t n);