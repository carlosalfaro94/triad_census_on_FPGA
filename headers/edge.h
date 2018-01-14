#include "defs.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
  *   @file edge.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to create and manage an edge of a graph. \nSince within
  *   project, we consider only directed graphs; edges
  *   must have a direction. \n
  *   In our implementation, edge information is saved twice, since each of the
  *   nodes keeps its adjacency list (see node.h).\n Each edge structure is therefore
  *   part of the node it belongs to, and contains the id of the other neigbhbor,
  *   and the direction.\n The direction is coded using the 2 less significant
  *   bits of the uint32_t used for storing the edge.\n '01' means that the edge goes
  *   out from the node it belongs, '10' means edge goes in, and '11' conveys
  *   bidirectionality.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

typedef uint32_t EDGE;   /*!< An edge is coded as an unsigned integer of 32 bits. */

/**
  * @brief Function that creates an EDGE from a node id and a direction
  * 
  * @param uint32_t edge_id: id of the neighbor node
  * @param DIRECTION dir: direction of the edge
  * 
  * @return EDGE: the new created edge
  *	
  **/
EDGE create_edge(uint32_t edge_id, DIRECTION dir);

/**
  * @brief Function that returns the neighbor id of an edge
  * 
  * @param EDGE e: The edge
  * 
  * @return uint32_t: The neighbor id
  *	
  **/
uint32_t get_neighbor_id(EDGE e);

void set_neighbor_id(EDGE* e, uint32_t id);


/**
  * @brief Function that returns the direction of an edge
  * 
  * @param EDGE e: The edge
  * 
  * @return DIRECTION: The direction of the edge
  *	
  **/
DIRECTION get_direction(EDGE e);

/**
  * @brief Function that sets the direction of an edge
  * 
  * @param EDGE e: The edge we want to modify
  * @param DIRECTION dir: The direction to set
  * 
  * @return STATUS: OK if the operation was successful\n
  					        ERR otherwise
  *	
  **/
STATUS set_direction (EDGE* e, DIRECTION dir);

/**
  * @brief Function that compares to edges based on the 
  * neighbor ids
  * 
  * @param const void* e1: First edge
  * @param const void* e2: Second edge
  * 
  * @return int: a value less than, equal to or greater than 0 \n
  * depending on whether e1 is smaller than, equal or greater \n
  * than e2, respectively
  *	
  **/
int comp_edges(const void* e1, const void* e2);

/**
  * @brief Function that inserts an edge in an adjacency lists
  * 
  * @param void* adj_list: array where to insert the edge
  * @param void* edge: Edge to insert
  * @param int pos: The position where to insert
  * 
  * @return None
  *	
  **/
void insert_edge(void* adj_list, void* e, int pos);

/**
  * @brief Function prints an edge
  * 
  * @param uint32_t in: the node id from where the node goes out
  * @param EDGE e: The edge to print
  * 
  * @return None
  *	
  **/
void print_edge(uint32_t in, EDGE e);