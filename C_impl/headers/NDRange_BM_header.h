#include "defs.h"


/**
  *   @file NDRange_BM_header.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to execute the NDRange BM kernels.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 17-12-2017
  **/



/**
  * @brief Function that computes the triad type of a triple of nodes
  * 
  * @param __constant EDGE_DEVICE* edges: Global edge array
  * @param NODE_DEVICE* u: First node of the triple
  * @param NODE_DEVICE* v: Second node of the triple
  * @param uint w_id: Id of the third node of the triple
  * 
  *	@return int: A number between 0 and 15, depending on the configuration 
  * of the triple
  *
  **/
short isoTricode(__constant EDGE_DEVICE* edges, NODE_DEVICE* u, NODE_DEVICE* v, unsigned int w_id);

/*EDGE_DEVICE functions*/
/**
  * @brief Function that returns the neighbor id of an edge
  * 
  * @param EDGE_DEVICE e: The edge
  * 
  * @return uint: The neighbor id
  *	
  **/
uint get_neighbor_id(EDGE_DEVICE e);

/**
  * @brief Function that returns the direction of an edge
  * 
  * @param EDGE_DEVICE e: The edge
  * 
  * @return DIRECTION: The direction of the edge
  *	
  **/
DIRECTION get_direction(EDGE_DEVICE e);

/**
  * @brief Function that turnup the direction of an edge
  * 
  * @param DIRECTION dir: The direction to turnup
  * 
  * @return DIRECTION: The opposite direction
  *	
  **/
DIRECTION turnup(DIRECTION dir);

/**
  * @brief Function that performs binary search over an ordered array 
  * 
  * @param __constant EDGE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  *	@param int first: the first index of the table
  * @param int last: the last index of the table
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int binary_search_adj_list(__constant EDGE_DEVICE* table, uint key, int first, int last);

/**
  * @brief Function that performs linear search over an unordered array 
  * 
  * @param __constant EDGE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  *	@param int first: the first index of the table
  * @param int last: the last index of the table
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int linear_search_adj_list(__constant EDGE_DEVICE* table, uint key, uint first, uint last);

/**
  * @brief Function that returns the position of a certain neighbor node 
  * within the edge array, given the first node and the neighbor id
  * 
  * @param __constant EDGE_DEVICE* edges: Edge array in which to search
  * @param NODE_DEVICE* n: The node of which we want to get the neighbor position
  * @param uint nid: The node id of the neighbor we want to search
  * 
  * @return int: The position within the edge array
  *
  **/
int get_neighbor_pos(__constant EDGE_DEVICE* edges, NODE_DEVICE* n, unsigned int nid);

/**
  * @brief Function that returns the direction of the edge present between two nodes (u,v)
  * 
  * @param __constant EDGE_DEVICE* edges: Edge array
  * @param NODE_DEVICE* u: The first node
  * @param uint v_id: The node id of the second node
  * 
  * @return DIRECTION: The direction of the edge between the nodes
  *	
  **/
DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, NODE_DEVICE* u, unsigned int v_id);
