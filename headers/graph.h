#include "node.h"

/**
  *   @file graph.h
  *
  *   @brief This file contains the definitions of the functions
  *   needed to create and manage a graph. \n
  *   In our implementation, the graph is read from a file which, in
  *   each of its lines, contains an edge of the graph in this format:\n
  *             src_node_id     tgt_node_id   \n
  *   In the structure, nodes can be ordered by ascending order of their ids.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

/**
 *  @struct GRAPH
 *  @brief This structure stores the necessary graph information
 */
typedef struct {
    NODE **nodes;   /*!< Array of pointers to nodes */
    uint32_t num_nodes;    /*!< Number of nodes in the graph (i.e length of the nodes array) */
} GRAPH;


/**
  * @brief Function that sets the global variable ordered to a certain value
  *        passed as a parameter
  * 
  * @param BOOL value: The boolean value we want to set.
  * 
  * @return: None 
  *
  **/
void set_ordered(BOOL value);

/**
  * @brief Function that reads the file containing the graph information,\n
  *        and creates the GRAPH structure, returning a pointer to it.
  * 
  * @param char* filename: The name of the file to read.
  * 
  * @return NODE*: A pointer to the  just created.
  *
  **/
GRAPH* read_graph_from_file(char *filename);

/**
  * @brief Function that inserts a node in the graph
  * 
  * @param GRAPH* g: The graph in which to insert
  * @param uint32_t node_id: The id of the node we want to insert
  * 
  * @return STATUS: OK if the insertion was successful.
  *                 ERR otherwise
  *
  **/
STATUS insert_node(GRAPH* g, uint32_t node_id);

/**
  * @brief Function that adds an edge to a certain node. To add an edge e,\n
  * we need to have the ids of the two nodes that the e connects; ant its \n
  * direction. The function basically adds the edge to the adjacency list of\n
  * the node with id node_id1, setting the neighbor id of the edge to node_id2.
  * 
  * @param GRAPH* g: The graph in which we want to add the edge
  * @param uint32_t node_id1: The id of one of the nodes of the edge
  * @param uint32_t node_id2: The id of the other id
  * @param DIRECTION dir: The direction of the edge: \nIN_OUT if it goes from id1 to id2.\n
                                                    OUT_IN if it goes from id2 to id1.
  *
  * @return STATUS: OK if the insertion was successful.
  *                 ERR otherwise
  *
  **/
STATUS add_edge(GRAPH* g, uint32_t node_id1, uint32_t node_id2, DIRECTION dir);

/**
  * @brief Function that checks whether a node is present in a graph
  * 
  * @param GRAPH* g: The graph in which to search.
  * @param uint32_t node_id: The id of the node we want to search.
  * 
  * @return BOOL: TRUE if the node is found.\n
  *               FALSE otherwise
  *
  **/
BOOL node_exists(GRAPH* g, uint32_t node_id);

/**
  * @brief Function that obtains the position of the node with id node_id within the \n
  * array of nodes of graph g.
  * 
  * @param GRAPH* g: The graph in which we want to search the position of the node.
  * @param uint32_t node_id: The id of the node we want to find the position of.
  *
  * @return STATUS: int the position in which the node is stored, if the node is actually present\n
  *                     -1 if the node is not found. 
  *
  **/
int get_node_pos(GRAPH* g, uint32_t node_id);

/**
  * @brief Function that obtains the number of nodes of a graph g
  * 
  * @param GRAPH* g: The graph
  *
  * @return uint32_t the number of nodes of the graph
  *
  **/
uint32_t get_num_nodes(const GRAPH* g);

/**
  * @brief Function that obtains the number of pairs (u,v) of nodes in graph g
  * 
  * @param GRAPH* g: The graph
  *
  * @return uint32_t the number of pairs in the graph
  *
  **/
uint32_t get_num_pairs(const GRAPH* g);

/**
  * @brief Function that obtains the number of triples (u,v,w) of nodes in graph g
  * 
  * @param GRAPH* g: The graph
  *
  * @return uint32_t the number of triples in the graph
  *
  **/
uint64_t get_num_triples(const GRAPH* g);

/**
  * @brief Function that obtains the number of edges of ha graph g
  * 
  * @param const GRAPH* g: The graph
  *
  * @return uint32_t the number of edges of the graph
  *
  **/
uint32_t get_num_edges(const GRAPH* g);

/**
  * @brief Function that obtains the node of graph g present in 
  * position pos.
  * 
  * @param GRAPH* g: The graph in which to search
  * @param int pos: The position of the node we want to obtain 
  *
  * @return NODE* the node we were looking for
  *
  **/
NODE* get_node_by_pos(const GRAPH* g, int pos);

/**
  * @brief Function that obtains the node of graph g that has
  * id node_id
  * 
  * @param GRAPH* g: The graph im which to search
  * uint32_t node_id: The position of the node we want to obtain 
  *
  * @return NODE* the node we were looking for
  *
  **/
NODE* get_node_by_id(GRAPH* g, uint32_t node_id);

/**
  * @brief Function that prints the information of graph
  * 
  * @param GRAPH* g: The graph to print
  * 
  * @return none
  *
  **/
void print_graph(GRAPH* g);

/**
  * @brief Function that destroys a graph, freeing the memory of
  *        its structures
  * 
  * @param GRAPH* g: the graph to destroy
  * 
  * @return none
  *
  **/
void destroy_graph(GRAPH *g);

/**
  * @brief Function that, given a graph, constructs the array of nodes and edges,\n
  *       which is a suitable saving format for processing the graph in the device.
  * 
  * @param GRAPH* g: the graph to convert
  * @param NODE_DEVICE** nodes: A pointer to the array of nodes (to be filled)
  * @param EDGE_DEVICE** edges: A pointer to the array of edges (to be filled)
  * @param uint32_t* num_nodes: The number of nodes, (to be filled)
  * @param uint32_t* num_edges: The number of edges (to be filled)
  * 
  * @return STATUS: OK if the arrays were constructed correctly, 
  *                 ERR otherwise
  *
  **/
STATUS convert_graph(GRAPH* g, NODE_DEVICE** nodes, EDGE_DEVICE** edges, uint32_t* num_nodes, uint32_t* num_edges);

/**
  * @brief Function that creates an array of tasks
  * 
  * @param GRAPH* g: the graph
  * @param NODE_DEVICE* nodes: The array of nodes
  * @param uint32_t* num_tasks: The number of tasks (to be filled)
  * 
  * @return TASK*: The array of tasks just created
  *
  **/
TASK* create_tasks_array(GRAPH* g, NODE_DEVICE* nodes, uint32_t* num_tasks);


void reorder_and_retag(GRAPH* g, int (*function) (const void*, const void*));