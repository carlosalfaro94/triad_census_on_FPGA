#include "../headers/edge.h"


/**
  *   @file edge.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file edge.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/



EDGE create_edge(uint32_t edge_id, DIRECTION dir){
	EDGE e;
	e = edge_id << 2;

	if (dir == IN_OUT){
		e = ((uint32_t) e) | 0x01;
	} else if (dir == OUT_IN){
		e = ((uint32_t) e) | 0x02;
	}

	return e;
}


uint32_t get_neighbor_id(EDGE e){
	return ((uint32_t) e) >> 2;
}

void set_neighbor_id(EDGE* e, uint32_t id){
	DIRECTION dir;
	dir = get_direction(*e);
	*e = create_edge(id, dir);
}

DIRECTION get_direction(EDGE e){
	if ((e & 0x03) == 0x01){
		return IN_OUT;
	} else if ((e & 0x03) == 0x02){
		return OUT_IN;
	} 
	return BIDIRECTIONAL;
}

STATUS set_direction (EDGE* e, DIRECTION dir){
	if (!e)
		return ERR;
	if (dir == IN_OUT){
		*e = ((uint32_t) *e) | 0x01;
	} else if (dir == OUT_IN){
		*e = ((uint32_t) *e) | 0x02;
	} else {
		return ERR;
	}
	return OK;
}

int comp_edges(const void* key, const void* e2){
	return *((uint32_t*) key) - get_neighbor_id(*((EDGE*) e2));
}

void insert_edge(void* adj_list, void* e, int pos){
	((EDGE*) adj_list)[pos] = *((EDGE*) e);
}

void print_edge(uint32_t in, EDGE e){
	uint32_t out;
	DIRECTION d;
	out = get_neighbor_id(e);
	d = get_direction(e);
	switch(d){
		case IN_OUT:
			printf("%u -> %u\t", in, out);
			break;
		case OUT_IN:
			printf("%u <- %u\t", in, out);
			break;
		default:
			printf("%u <-> %u\t", in, out);
	}
}