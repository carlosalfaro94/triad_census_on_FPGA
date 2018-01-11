#include "edge.h"
#include "helpers.h"


/**
  *   @file node.h
  *
  *   @brief This file contains the definitions of the functions \n
  *   needed to create and manage a node of a graph. \n
  *   In our implementation, the graph is basically a list of nodes \n
  *   which are identified uniquely with and unsigned integer. Each \n
  *   node keeps track of its adjacency list, bi means of an array of \n
  *   edges. Since each edge connects two nodes, edge information will \n
  *   be duplicated.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

/**
 *  @struct NODE
 *  @brief This structure stores the information of a node of the graph.
 */
typedef struct {
	uint32_t node_id;    /*!< The id of the node */
	EDGE* adj_list;      /*!< Pointer to the adjacency list of the node */
	uint32_t num_neighbors;  /*!< The number of neighbors of the node (i.e the length of the adjacency list)*/
  uint32_t degree;         /*!< The degree of the node */
} NODE;

/**
  * @brief Function that creates a new node, allocating memory for its fields
  * and initializing them. The node_id is set to the parameter passed, while the 
  * adj_list is set to NULL and the num_neighbors is set to 0
  * 
  * @param uint32_t node_id: The id of the node  
  * 
  *	@return NODE*: A pointer to the node just created.
  *
  **/
NODE* create_node(uint32_t node_id);

/**
  * @brief Function that creates a new node, allocating memory for its fields
  * and initializing them. The node_id is set to the parameter passed, while the 
  * adj_list is set to NULL and the num_neighbors is set to 0
  * 
  * @param NODE* n: The node in which we want to insert the edge
  * @param uint32_t neighbor_id: The node id of the neighbor we want to insert
  * @param DIRECTION dir: The direction of the edge we want to insert 
  * 
  *	@return STATUS: OK if the insertion was successful
  	                ERR otherwise 
  *
  **/
STATUS add_edge_to_node(NODE* n, uint32_t neighbor_id, DIRECTION dir);


/** Getter functions **/

/**
  * @brief Function that returns the position of a certain neighbor node 
  * within the adjacency list of the node, given the neighbor id
  * 
  * @param NODE* n: The node of which we want to get the neighbor position
  * @param uint32_t neighbor_id: The node id of the neighbor we want to search
  * 
  * @return int: The position within the adjacency list
  *
  **/
int get_neighbor_pos(NODE* n, uint32_t neighbor_id);

/**
  * @brief Function that returns id of a certain node
  * 
  * @param NODE* n: The node of which we want to discover the id
  * 
  * @return uint32_t: the node id
  *
  **/
uint32_t get_node_id(NODE* n);

/**
  * @brief Function that the edge present in a certain position\n
  * of the adjacency list of a node
  * 
  * @param NODE* n: The node from which we want to get the edge
  * @param int pos: The position within the adjacency list
  * 
  * @return EDGE: the edge
  *
  **/
EDGE get_edge(NODE* n, int pos);

/**
  * @brief Function that returns the adjacency list of a certain node
  * 
  * @param NODE* n: The node of which we want to get the adjacency list
  * 
  * @return EDGE*: The adjacency list
  *
  **/
EDGE* get_adj_list(NODE* n);

/**
  * @brief Function that returns the number of neighbors a certain node has
  * 
  * @param NODE* n: The node of which we want to discover the number of neighbors
  * 
  * @return int the number of neighbors
  *
  **/
int get_num_neighbors(NODE* n);


/**
  * @brief Function that returns the degree of a certain node
  * 
  * @param NODE* n: The node of which we want to discover the degree
  * 
  * @return int the degree
  *
  **/
int get_degree(NODE* n);

/**
  * @brief Function that compares two nodes (useful for binary search and insertion sort).\n
  * The comparison is made based on the node ids
  * 
  * @param const void* key: The key we want to compare to the other node.
  * It is a pointer to a node id.
  * @param const void* pnode: A pointer to the node we want to compare to the key
  * 
  * @return int A number that is less than, greater than or equal to 0, depending on weather
  * the key is less than, greather than or equal to the pnode.
  *
  **/
int comp_nodes_by_id(const void* key, const void* pnode);

/**
  * @brief Function that compares two nodes (useful for binary search and insertion sort).\n
  * The comparison is made based on the number of neighbors of each node
  * 
  * @param const void* key: The key we want to compare to the other node.
  * It is a pointer to the number of neighbors of a node.
  * @param const void* pnode: A pointer to the node we want to compare to the key
  * 
  * @return int A number that is less than, greater than or equal to 0, depending on weather
  * the key is less than, greather than or equal to the pnode.
  *
  **/
int comp_nodes_by_degree(const void* key, const void* pnode);

/**
  * @brief Function that compares two ids (useful for binary search and insertion sort).
  * 
  * @param const void* key: The key we want to compare to the pointer.
  * It is a pointer an id.
  * @param const void* pid: A pointer to the id we want to compare to the key
  * 
  * @return int A number that is less than, greater than or equal to 0, depending on weather
  * the key is less than, greather than or equal to the id.
  *
  **/
int comp_ids(const void* key, const void* pid);

/**
  * @brief Function that inserts a node in a list for nodes \n(useful for the helper function
  *        insert_sort)
  * 
  * @param void* nodes: The list of nodes in which we want to insert the new node
  * @param const void* node: The node to insert
  * @param int pos: The position in which to insert
  * 
  * @return None
  *
  **/
void insert_nodes(void* nodes, void* node, int pos);

/**
  * @brief Function that constructs the ordered union of the adjacency lists of two nodes \n
  *        u and v, storing only the neighbor ids and avoiding the insertion of \n
  *        u and v (in case they were present in the lists).\n
  *        The function also fills the parameter card_S with the cardinality of the \n
  *        union.
  * 
  * @param int* card_S: The cardinality of the union (to be filled by the function)
  * @param NODE* u: The first node
  * @param NODE* v: The second node
  * 
  * @return uint32_t* The array containing the node ids of the union.
  *
  **/
uint32_t* union_adj_lists_ordered(int* card_S, NODE* u, NODE* v);

/**
  * @brief Function that constructs the not ordered union of the adjacency lists of two nodes \n
  *        u and v, storing only the neighbor ids and avoiding the insertion of \n
  *        u and v (in case they were present in the lists).\n
  *        The function also fills the parameter card_S with the cardinality of the\n
  *        union.
  * 
  * @param int* card_S: The cardinality of the union (to be filled by the function)
  * @param NODE* u: The first node
  * @param NODE* v: The second node
  * 
  * @return uint32_t* The array containing the node ids of the union.
  *
  **/
uint32_t* union_adj_lists_not_ordered(int* card_S, NODE* u, NODE* v);


/**
  * @brief Function that constructs the union of the adjacency lists of two nodes\n
  *        u and v, storing only the neighbor ids and avoiding the insertion of \n
  *        u and v (in case they were present in the lists).\n
  *        The function also fills the parameter card_S with the cardinality of the\n
  *        union.
  * 
  * @param NODE* u: The first node
  * @param NODE* v: The second node
  * 
  * @return DIRECTION: The direction between the two nodes:\n
  *                      IN_OUT, OUT_IN, or BIDIRECTIONAL in case they are connected.\n
  *                      NONE in case they are not connected
  *
  **/
DIRECTION get_dir_between_nodes(NODE* u, NODE* v);

/**
  * @brief Function that checks if two nodes u and v are connected.
  * 
  * @param NODE* u: The first node
  * @param NODE* v: The second node
  * 
  * @return BOOL TRUE if the nodes u and v are not connected.\n
  *              FALSE if u and v are connected.
  *
  **/
BOOL not_connected(NODE* u, NODE* v);

/**
  * @brief Function that prints the information of a node
  * 
  * @param NODE* n: the node to print
  * 
  * @return none
  *
  **/
void print_node(NODE* n);

/**
  * @brief Function that destroys a node, freeing the memory of \n
  *        its structures
  * 
  * @param NODE* n: the node to destroy
  * 
  * @return none
  *
  **/
void destroy_node(NODE *n);
