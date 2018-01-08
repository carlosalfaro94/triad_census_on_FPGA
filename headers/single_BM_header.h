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

/*Function to look up the tricode in a triple of nodes*/
short isoTricode(EDGE_DEVICE* adj_list_u, EDGE_DEVICE* adj_list_v, NODE_DEVICE* u, NODE_DEVICE* v, uint w_id);

/*EDGE_DEVICE functions*/

uint get_neighbor_id(EDGE_DEVICE e);

DIRECTION get_direction(EDGE_DEVICE e);

DIRECTION turnup(DIRECTION dir);

int binary_search_adj_list(EDGE_DEVICE* table, uint key, int first, int last);

int linear_search_adj_list(EDGE_DEVICE* table, uint key, uint first, uint last);

/*NODE_DEVICE functions*/

uint get_node_id(__constant NODE_DEVICE* n);

uint get_first(__constant NODE_DEVICE* n);

uint get_last(__constant NODE_DEVICE* n);

/*Functions to copy data to local memory*/

NODE_DEVICE copy_node(__constant NODE_DEVICE* source);

ushort copy_edge_list(__constant EDGE_DEVICE* edges, EDGE_DEVICE* local_edges, NODE_DEVICE* node);

int get_neighbor_pos(EDGE_DEVICE* edges, NODE_DEVICE* n, uint nid);

DIRECTION get_dir_between_nodes(EDGE_DEVICE* edges, NODE_DEVICE* u, uint v_id);

/*NODE_LIST search functions*/

uint get_node_pos(__constant NODE_DEVICE* node_list, uint node_id, uint num_nodes);

int binary_search_node_list_norec(__constant NODE_DEVICE* table, uint key, uint first, uint last);

int linear_search_node_list(__constant NODE_DEVICE* table, uint key, uint num_nodes);