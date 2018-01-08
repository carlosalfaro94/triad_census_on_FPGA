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

short isoTricode(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v, __constant NODE_DEVICE* w);

unsigned int get_neighbor_id(EDGE_DEVICE e);

DIRECTION get_direction(EDGE_DEVICE e);

int linear_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, unsigned int first, unsigned int last);

int binary_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, int first, int last);

unsigned int get_node_id(__constant NODE_DEVICE* n);

int get_neighbor_pos(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* n, unsigned int nid);

BOOL not_connected(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v);

DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v);