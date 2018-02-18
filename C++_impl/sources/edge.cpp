

#include "../headers/edge.h"

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

DIRECTION get_direction(EDGE e){
	if ((e & 0x03) == 0x01){
		return IN_OUT;
	} else if ((e & 0x03) == 0x02){
		return OUT_IN;
	} 
	return BIDIRECTIONAL;
}

DIRECTION turnup(DIRECTION dir){
	if (dir == IN_OUT){
		return OUT_IN;
	} else if (dir == OUT_IN){
		return IN_OUT;
	} else {
		return dir;
	}
}

bool comp_edges(EDGE e1, EDGE e2){
	return get_neighbor_id(e1) < get_neighbor_id(e2);
}

void print_edge(uint32_t in, EDGE e){
	uint32_t out;
	DIRECTION d;
	out = get_neighbor_id(e);
	d = get_direction(e);
	switch(d){
		case IN_OUT:
			std::cout << in << " -> " << out << "\t";
			break;
		case OUT_IN:
			std::cout << in << " <- " << out << "\t";
			break;
		default:
			std::cout << in << " <-> " << out << "\t";
	}
}
