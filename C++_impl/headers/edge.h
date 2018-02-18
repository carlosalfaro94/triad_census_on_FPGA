#include <iostream>
#include <stdlib.h>
#include <inttypes.h>

#include "defs.h"

typedef uint32_t EDGE;


EDGE create_edge(uint32_t edge_id, DIRECTION dir);

uint32_t get_neighbor_id(EDGE e);

DIRECTION get_direction(EDGE e);

DIRECTION turnup(DIRECTION dir);

bool comp_edges(EDGE e1, EDGE e2);

void print_edge(uint32_t in, EDGE e);