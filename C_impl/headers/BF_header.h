#include "defs.h"

/**
  *   @file BF_header.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to execute the Brute Force kernels.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 17-12-2017
  **/

/**
  * @brief Function that computes the triad type of a triple of nodes
  * 
  * @param __constant EDGE_DEVICE* edges: Global edge array
  * @param __constant NODE_DEVICE* u: First node of the triple
  * @param __constant NODE_DEVICE* v: Second node of the triple
  * @param _constant NODE_DEVICE*  w: Third node of the triple
  * 
  *	@return int: A number between 0 and 15, depending on the configuration 
  * of the triple
  *
  **/
short isoTricode(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v, __constant NODE_DEVICE* w);



/**
  * @brief Function that returns the neighbor id of an edge
  * 
  * @param EDGE_DEVICE e: The edge
  * 
  * @return uint: The neighbor id
  *	
  **/
unsigned int get_neighbor_id(EDGE_DEVICE e);

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
  * @brief Function that performs linear search over an unordered array 
  * 
  * @param __constant EDGE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  *	@param uint first: the first index of the table
  * @param uint last: the last index of the table
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int linear_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, unsigned int first, unsigned int last);

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
int binary_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, int first, int last);

/**
  * @brief Function that returns id of a certain node
  * 
  * @param NODE* n: The node of which we want to discover the id
  * 
  * @return uint: the node id
  *
  **/
unsigned int get_node_id(__constant NODE_DEVICE* n);

/**
  * @brief Function that returns the position of a certain neighbor node 
  * within the edge array, given the first node and the neighbor id
  * 
  * @param __constant EDGE_DEVICE* edges: Edge array in which to search
  * @param __constant NODE_DEVICE* n: The node of which we want to get the neighbor position
  * @param uint nid: The node id of the neighbor we want to search
  * 
  * @return int: The position within the edge array
  *
  **/
int get_neighbor_pos(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* n, unsigned int nid);

/**
  * @brief Function that checks if two nodes u and v are connected.
  * 
  * @param __constant EDGE_DEVICE* edges: The global edge array
  * @param __constant NODE_DEVICE* u: The first node
  * @param __constant NODE_DEVICE* v: The second node
  * 
  * @return BOOL TRUE if the nodes u and v are not connected.\n
  *              FALSE if u and v are connected.
  *
  **/
BOOL not_connected(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v);

/**
  * @brief Function that returns the direction of the edge present between two nodes (u,v)
  * 
  * @param __constant EDGE_DEVICE* edges: Global edge array
  * @param __constant NODE_DEVICE* u: The first node
  * @param __constant NODE_DEVICE* v: The second node
  * 
  * @return DIRECTION: The direction of the edge between the nodes
  *	
  **/
DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v);