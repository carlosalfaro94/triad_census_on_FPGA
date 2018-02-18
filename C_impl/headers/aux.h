#include "../headers/triads.h"
#include <string.h>

/**
  *   @file aux.h
  *
  *   @brief This file contains the definitions of some auxiliary functions\n
  *	  		 used by the main program run the program.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 18-12-2017
  **/

/**
  * @brief Function that displays a summary of graph g printing its number of nodes, edges and triads.
  *
  * @param GRAPH* g: The input graph.
  *
  *	@return None
  *
  **/
void display_graph_summary(GRAPH* g);

/**
  * @brief Function that checks the name of a kernel to discover if the kernel expects ordered input
  *
  * @param char* name_kernel: The name of the kernel.
  *
  *	@return BOOL: TRUE if the kernel expects ordered input.\n
  *				  FALSE otherwise
  *
  **/
BOOL is_ordered_kernel(char* name_kernel);

/**
  * @brief Function that checks the name of a kernel to discover if the kernel is a NDRange kernel
  *
  * @param char* name_kernel: The name of the kernel.
  *
  *	@return BOOL: TRUE if it is a NDRange kernel.\n
  *				  FALSE otherwise
  *
  **/
BOOL is_NDRange_kernel(char* name_kernel);

/**
  * @brief Function that checks the name of a kernel to discover if the kernel implements a BM algorithm
  *
  * @param char* name_kernel: The name of the kernel.
  *
  *	@return BOOL: TRUE if the kernel implements BM.\n
  *				  FALSE otherwise
  *
  **/
BOOL is_BM_kernel(char* name_kernel);

/**
  * @brief Function that writes performance data to a file
  *
  * @param char* times_name: The name of the file in which to write the results.
  * @param GRAPH* g: The graph under analysis.
  * @param double reading_time: Graph reading time.
  * @param double execution time: Triad census execution time.
  *
  *	@return None
  *
  **/
void write_times(char* times_name, GRAPH* g, double reading_time, double execution_time);

/**
  * @brief Function that randomly generates a graph and writes it to a file
  *
  * @param unsigned int num_nodes: Number of nodes desired.
  * @param unsigned int num_edges: Number of edges desired.
  * @param unsigned int seed: The seed for rand() function
  * @param char* namefile: The name of the file in which to write the graph.
  * 
  *	@return None
  *
  **/
void rand_graph_generation(unsigned int num_nodes, unsigned int num_edges, unsigned int seed, char* namefile);