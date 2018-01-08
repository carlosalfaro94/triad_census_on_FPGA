#include "BF_header.h"

__kernel void NDRange_BF(__constant NODE_DEVICE* restrict node_list, __constant EDGE_DEVICE* restrict edge_list, __global CENSUS* census_array, uint num_nodes){
	
	unsigned int i,j,k;
	short tritype;

	i = get_global_id(0);
	//for(j=0;j<16;j++){
	//	census_array[i].census[j] = 0;
	//}
	for (j=i+1;j<num_nodes;j++){
		for(k=j+1;k<num_nodes;k++){
			tritype = isoTricode(edge_list, &(node_list[i]), &(node_list[j]), &(node_list[k]));
			census_array[i].census[tritype]++;
		}
	}
}



/*AUXILIARY FUNCTIONS*/

short isoTricode(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v, __constant NODE_DEVICE* w){
	DIRECTION dir1, dir2, dir3;
 	int code;

	dir1 = get_dir_between_nodes(edges,u,v);
	dir2 = get_dir_between_nodes(edges,v,w);
	dir3 = get_dir_between_nodes(edges,w,u);

	code = (int) dir3 * 16 + (int) dir2 * 4 + (int) dir1;

	switch(code){
		case 0b000000:
			return 0;
		case 0b000001:
		case 0b000010:
		case 0b000100:
		case 0b001000:
		case 0b010000:
		case 0b100000:
			return 1;
		case 0b000011:
		case 0b001100:
		case 0b110000:
			return 2;
		case 0b100001:
		case 0b000110:
		case 0b011000:
			return 3;
		case 0b010010:
		case 0b001001:
		case 0b100100:
			return 4;
		case 0b100010:
		case 0b001010:
		case 0b101000:
		case 0b010001:
		case 0b000101:
		case 0b010100:
			return 5;
		case 0b010011:
		case 0b001011:
		case 0b001101:
		case 0b101100:
		case 0b110100:
		case 0b110010:
			return 6;
		case 0b100011:
		case 0b000111:
		case 0b001110:
		case 0b011100:
		case 0b111000:
		case 0b110001:
			return 7;
		case 0b010110:
		case 0b100110:
		case 0b101001:
		case 0b100101:
		case 0b011001:
		case 0b011010:
			return 8;
		case 0b010101:
		case 0b101010:
			return 9;
		case 0b001111:
		case 0b110011:
		case 0b111100:
			return 10;
		case 0b110110:
		case 0b101101:
		case 0b011011:
			return 11;
		case 0b011110:
		case 0b111001:
		case 0b100111:
			return 12;
		case 0b101110:
		case 0b111010:
		case 0b101011:
		case 0b011101:
		case 0b110101:
		case 0b010111:
			return 13;
		case 0b111110:
		case 0b111101:
		case 0b111011:
		case 0b110111:
		case 0b101111:
		case 0b011111:
			return 14;
		case 0b111111:
			return 15;
		default:
			return -1;

	}
	return -1;
}


unsigned int get_neighbor_id(EDGE_DEVICE e){
	return ((unsigned int) e) >> 2;
}

DIRECTION get_direction(EDGE_DEVICE e){
	if ((e & 0x03) == 0x01){
		return IN_OUT;
	} else if ((e & 0x03) == 0x02){
		return OUT_IN;
	} 
	return BIDIRECTIONAL;
}


int linear_search_adj_list(__constant EDGE_DEVICE* table, unsigned int key, unsigned int first, unsigned int last){
	int i;
	int result = -1;
	for (i=first;i<=last;i++){
		if (key == get_neighbor_id(table[i])){
			result = i;
		}
	}
	return result;
}

unsigned int get_node_id(__constant NODE_DEVICE* n){
	return n->node_id;
}

int get_neighbor_pos(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* n, unsigned int nid){
	return linear_search_adj_list(edges, nid, n->first_ind, n->last_ind);
}

BOOL not_connected(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v){
	return (-1 == get_neighbor_pos(edges, u, get_node_id(v)));
}


DIRECTION get_dir_between_nodes(__constant EDGE_DEVICE* edges, __constant NODE_DEVICE* u, __constant NODE_DEVICE* v){
	unsigned int pos;
	if (not_connected(edges,u,v)){
		return NONE;
	}
	pos = get_neighbor_pos(edges, u, get_node_id(v));
	return get_direction(edges[pos]);
}
