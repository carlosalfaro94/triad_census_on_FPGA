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

short isoTricode(__constant EDGE_DEVICE* edges, NODE_DEVICE* u, NODE_DEVICE* v, unsigned int w_id);

/*EDGE_DEVICE functions*/

uint get_neighbor_id(EDGE_DEVICE e);

DIRECTION get_direction(EDGE_DEVICE e);

DIRECTION turnup(DIRECTION dir);


int binary_search_adj_list(__constant EDGE_DEVICE* table, uint key, int first, int last);


int linear_search_adj_list(__constant EDGE_DEVICE* table, uint key, uint first, uint last);


int get_neighbor_pos(__constant EDGE_DEVICE* edges, NODE_DEVICE* n, unsigned int nid);


DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, NODE_DEVICE* u, unsigned int v_id);
