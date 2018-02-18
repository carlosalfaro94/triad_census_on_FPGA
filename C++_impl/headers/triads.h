
#include "graph.h"
#include <iterator>
#include <algorithm>
#include <utility>

#define NUM_TRIADS 16

uint64_t* triad_census_BF(NODE_DEVICE* nodes, EDGE_DEVICE* edges, uint32_t num_nodes);

uint64_t* triad_census_BM(NODE_DEVICE* nodes, EDGE_DEVICE* edges, uint32_t num_nodes);

void print_triad_census(uint64_t* triads);

short isoTricode(EDGE_DEVICE* edges, NODE_DEVICE* u, NODE_DEVICE* v, uint32_t w_id);

int get_neighbor_pos(EDGE_DEVICE* edges, NODE_DEVICE* u, uint32_t v_id);

int binary_search_adj_list(EDGE_DEVICE* table, uint32_t key, int first, int last);

DIRECTION get_dir_between_nodes(EDGE_DEVICE* edges, NODE_DEVICE* u, uint32_t v_id);

uint64_t num_total_triads(uint64_t num_nodes);