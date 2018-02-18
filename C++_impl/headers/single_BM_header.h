#include "defs.h"


/**
  *   @file single_BM_header.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to execute the single BM kernels.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 17-12-2017
  **/


/**
  * @brief Function that computes the triad type of a triple of nodes
  * 
  * @param EDGE_DEVICE* adj_list_u: Adjacency list of u
  * @param NODE_DEVICE* u: First node of the triple
  * @param NODE_DEVICE* v: Second node of the triple
  * @param uint w_id: Id of the third node of the triple
  * 
  *	@return int: A number between 0 and 15, depending on the configuration 
  * of the triple
  *
  **/
short isoTricode(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v, uint w_id);

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
  * @brief Function that performs binary search over an ordered edge list 
  * 
  * @param EDGE_DEVICE* table: The array in which to perform the search
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
  * @brief Function that performs linear search over an unordered edge list 
  * 
  * @param EDGE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  * @param uint last: the last index of the table
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int binary_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, int first, int last);

/*NODE_DEVICE functions*/
/**
  * @brief Function that returns id of a certain node
  * 
  * @param __constant NODE_DEVICE* n: The node of which we want to discover the id
  * 
  * @return uint: the node id
  *
  **/
uint get_node_id(__constant NODE_DEVICE* n);

/**
  * @brief Function that returns the index of the beginning of the adjacency list of a certain node
  * 
  * @param __constant NODE_DEVICE* n: The node
  * 
  * @return uint: the first index
  *
  **/
uint get_first(__constant NODE_DEVICE* n);

/**
  * @brief Function that returns the index of the final of the adjacency list of a certain node
  * 
  * @param __constant NODE_DEVICE* n: The node
  * 
  * @return uint: the last index
  *
  **/
uint get_last(__constant NODE_DEVICE* n);

/*Functions to copy data to local memory*/
/**
  * @brief Function that copies the information of a global node to local memory
  * 
  * @param __constant NODE_DEVICE* source: The node we want to copy
  * 
  * @return NODE DEVICE: The node saved in local memory
  *
  **/
NODE_DEVICE copy_node(__constant NODE_DEVICE* source);

/**
  * @brief Function that copies the information of the adjacency list of a certain node to local memory
  * 
  * @param __constant EDGE_DEVICE* edges: Global edge array
  * @param EDGE_DEVICE* local_ edges: Local array containing the adjacency list (to be filled by the function)
  * @param NODE_DEVICE* node: The node associated to the adjacency list we want to copy
  * 
  * @return ushort: The length of the adjacency list
  *
  **/
ushort copy_edge_list(__constant EDGE_DEVICE* edges, EDGE_DEVICE* local_edges, NODE_DEVICE* node);


/**
  * @brief Function that returns the position of a certain neighbor node 
  * within the edge array, given the first node and the neighbor id
  * 
  * @param EDGE_DEVICE* edges: Edge array in which to search
  * @param NODE_DEVICE* n: The node of which we want to get the neighbor position
  * @param uint nid: The node id of the neighbor we want to search
  * 
  * @return int: The position within the edge array
  *
  **/
int get_neighbor_pos(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* n, uint nid);

/**
  * @brief Function that returns the direction of the edge present between two nodes (u,v)
  * 
  * @param EDGE_DEVICE* edges: Edge array
  * @param NODE_DEVICE* u: The first node
  * @param uint v_id: The node id of the second node
  * 
  * @return DIRECTION: The direction of the edge between the nodes
  *	
  **/
DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, uint v_id);

/*NODE_LIST search functions*/

/**
  * @brief Function that obtains the position of the node with id node_id within the \n
  * array of nodes of graph g.
  * 
  * @param __constant NODE_DEVICE* node_list: The node list in which we want to search the position of the node.
  * @param uint node_id: The id of the node we want to find the position of.
  * @param uint num_nodes: The length of the node list
  *
  * @return STATUS: uint the position in which the node is stored
  *
  **/
#if 0
uint get_node_pos(__constant NODE_DEVICE* node_list, uint node_id, uint num_nodes);

/**
  * @brief Function that performs binary search over an ordered node list
  * 
  * @param __constant NODE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  *	@param uint first: the first index of the table
  * @param uint last: the last index of the table
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int binary_search_node_list_norec(__constant NODE_DEVICE* table, uint key, uint first, uint last);

/**
  * @brief Function that performs linear search over an unordered node list 
  * 
  * @param __constant NODE_DEVICE* table: The array in which to perform the search
  * @param uint key: The key to search within the array
  *	@param uint num_nodes: length of the array
  *
  *	@return int the position in which the key is stored, if it is found\n
  *				-1 otherwise
  *
  **/
int linear_search_node_list(__constant NODE_DEVICE* table, uint key, uint num_nodes);
#endif