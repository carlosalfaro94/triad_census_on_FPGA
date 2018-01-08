#include "single_BM_header.h"


__kernel void single_BM(__constant NODE_DEVICE* restrict node_list, __constant EDGE_DEVICE* restrict edge_list, __global ulong* census, uint num_nodes){
	ushort j,k;
	uint n,m;
	ushort card_union;
	uint u_id, v_id, w_id;
	ushort tritype;
	ushort deg_u, deg_v;  //degrees of u & v
	uint local_census[NUM_TRIADS];
	EDGE_DEVICE adj_list_u[MAX_NEIGHBORS], adj_list_v[MAX_NEIGHBORS];
	NODE_DEVICE u, v;


	for (n=0; n<num_nodes; n++){  
		u = copy_node(&(node_list[n]));    /*n is the index of u within node_list*/
		u_id = u.node_id;
		deg_u = copy_edge_list(edge_list, adj_list_u, &u);
		
		for (j=0;j<deg_u;j++){
			v_id = get_neighbor_id(adj_list_u[j]);
			if (u_id < v_id){
				m = get_node_pos(node_list, v_id, num_nodes); /*m is the index of v within node_list*/
				v = copy_node(&(node_list[m])); 
				deg_v = copy_edge_list(edge_list, adj_list_v, &v);

				/*PERFORM IN-PLACE UNION*/
				card_union = 0;
				for (k=0;k<deg_u;k++){
					w_id = get_neighbor_id(adj_list_u[k]);
					if (v_id != w_id){
						card_union++;
						if (u_id < w_id && (v_id < w_id || -1 == get_neighbor_pos(adj_list_u, &u, w_id))){
							tritype = isoTricode(adj_list_u, adj_list_v, &u, &v, w_id);
							local_census[tritype]++;	
						}
					}
				}
				for (k=0;k<deg_v;k++){
					w_id = get_neighbor_id(adj_list_v[k]);
					if (-1 == get_neighbor_pos(adj_list_u, &u, w_id) && u_id != w_id){
						card_union++;
						if (u_id < w_id){
							tritype = isoTricode(adj_list_u, adj_list_v, &u, &v, w_id);
							local_census[tritype]++;	
						}
					}
				}
				if (BIDIRECTIONAL == get_direction(adj_list_u[j])){
					tritype = 2;
				} else {
					tritype = 1;
				}
				local_census[tritype] += num_nodes - card_union - 2;
			}
		}
	}

	for (tritype = 0;tritype<NUM_TRIADS;tritype++){
		census[tritype] = local_census[tritype];
	}
}


/*AUXILIARY FUNCTIONS*/
short isoTricode(EDGE_DEVICE* adj_list_u, EDGE_DEVICE* adj_list_v, NODE_DEVICE* u, NODE_DEVICE* v, uint w_id){
	DIRECTION dir1, dir2, dir3;
 	int code;

 	dir1 = get_dir_between_nodes(adj_list_u, u, v->node_id);
 	dir2 = get_dir_between_nodes(adj_list_v, v, w_id);
 	dir3 = turnup(get_dir_between_nodes(adj_list_u, u,w_id));

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


uint get_neighbor_id(EDGE_DEVICE e){
	return ((uint) e) >> 2;
}

DIRECTION get_direction(EDGE_DEVICE e){
	if ((e & 0x03) == 0x01){
		return IN_OUT;
	} else if ((e & 0x03) == 0x02){
		return OUT_IN;
	} 
	return BIDIRECTIONAL;
}

DIRECTION turnup(DIRECTION dir){
	if (dir == IN_OUT)
		return OUT_IN;
	else if (dir == OUT_IN)
		return IN_OUT;
	return dir;
}

int linear_search_adj_list(EDGE_DEVICE* table, uint key, uint first, uint last){
	int i;
	int result = -1;
	for (i=first;i<=last;i++){
		if (key == get_neighbor_id(table[i])){
			result = i;
		}
	}
	return result;
}

uint get_node_id(__constant NODE_DEVICE* n){
	return n->node_id;
}

uint get_first(__constant NODE_DEVICE* n){
	return n->first_ind;
}

uint get_last(__constant NODE_DEVICE* n){
	return n->last_ind;
}

NODE_DEVICE copy_node(__constant NODE_DEVICE* source){
	NODE_DEVICE dest;
	dest.node_id = get_node_id(source);
	dest.first_ind = get_first(source);
	dest.last_ind = get_last(source);
	return dest;
}

ushort copy_edge_list(__constant EDGE_DEVICE* edges, EDGE_DEVICE* local_edges, NODE_DEVICE* node){
	int i,j;
	for(i=0,j=node->first_ind; j<=node->last_ind;i++,j++){
		local_edges[i] = edges[j];
	}
	node->first_ind = 0;
	node->last_ind = i-1;
	return i;
}

int get_neighbor_pos(EDGE_DEVICE* edges, NODE_DEVICE* n, uint nid){
	return linear_search_adj_list(edges, nid, n->first_ind, n->last_ind);
}

DIRECTION get_dir_between_nodes(EDGE_DEVICE* edges, NODE_DEVICE* u, uint v_id){
	int pos;
	pos = get_neighbor_pos(edges, u, v_id);
	if (pos == -1)
		return NONE;
	return get_direction(edges[pos]);
}

uint get_node_pos(__constant NODE_DEVICE* node_list, uint node_id, uint num_nodes){
	return linear_search_node_list(node_list, node_id, num_nodes);
}

int linear_search_node_list(__constant NODE_DEVICE* table, uint key, uint num_nodes){
	int i;
	int result = -1;
	for (i=0;i<num_nodes;i++){
		if (key == get_node_id(&(table[i]))){
			result = i;
		}
	}
	return result;
}