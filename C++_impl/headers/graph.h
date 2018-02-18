#include "edge.h"

#include <map>
#include <set>



struct classcomp {
  bool operator() (const EDGE& e1, const EDGE& e2) const
  {return get_neighbor_id(e1) < get_neighbor_id(e2);}
};


typedef std::set<EDGE, classcomp> LIST_EDGES;

typedef std::map<uint32_t, LIST_EDGES> GRAPH;


GRAPH read_graph_from_file(char* filename);

uint32_t get_num_nodes(GRAPH g);

uint32_t get_num_edges(GRAPH g);

uint32_t get_max_degree(GRAPH g);

STATUS convert_graph(GRAPH g, NODE_DEVICE** nodes, EDGE_DEVICE** edges, uint32_t* num_nodes, uint32_t* num_edges);

TASK* create_tasks_array(GRAPH g, NODE_DEVICE* nodes, uint32_t* num_tasks);

void print_graph(const GRAPH g);



