
#include "../headers/triads.h"

const char* triad_codes[NUM_TRIADS] = {"003","012","102","021D","021U","021C","111D","111U","030T","030C","201","120D","120U","120C","210","300"};


uint64_t* triad_census_BF(NODE_DEVICE* nodes, EDGE_DEVICE* edges, uint32_t num_nodes){
	uint64_t* triads;
    uint32_t i, j, k;
    int tritype;

    triads = (uint64_t*) calloc(NUM_TRIADS, sizeof(uint64_t));

    if (NULL == triads){
        printf("Error allocating memory for triad census array\n");
        return NULL;
    }


    for(i=0;i<num_nodes;i++){
    	for(j=i+1;j<num_nodes;j++){
    		for(k=j+1;k<num_nodes;k++){
    			tritype = isoTricode(edges, &(nodes[i]), &(nodes[j]), nodes[k].node_id);
    			triads[tritype]++;
    		}
    	}
    }

    return triads;
}


uint64_t* triad_census_BM(NODE_DEVICE* nodes, EDGE_DEVICE* edges, uint32_t num_nodes){
	uint64_t* triads;
	int tritype;
	uint32_t i,j,k,l;
    NODE_DEVICE u, v, z;
	uint32_t u_id, v_id, w_id, id1, id2, id;
    uint32_t card_union;
	uint64_t sum;


	triads = (uint64_t*) calloc(NUM_TRIADS, sizeof(uint64_t));

	if (NULL == triads){
		printf("Error allocating memory for triad census array\n");
		return NULL;
	}

    for (i=0;i<num_nodes; i++){
    	u_id = i;
        u = nodes[u_id];
        for (j=u.first_ind;j<=u.last_ind;j++){
            v_id = get_neighbor_id(edges[j]);
            if (u_id < v_id){
	            v = nodes[v_id];
	            k = u.first_ind;
	            l = v.first_ind;
	            card_union = 0;
	            while(k <= u.last_ind && l <= v.last_ind){
	                id1 = get_neighbor_id(edges[k]);
	                id2 = get_neighbor_id(edges[l]);
	                if (id1 == id2){
	                    w_id = id1;
	                    k++;
	                    l++;
	                } else if (id1<id2){
	                    w_id = id1;
	                    k++;
	                } else {
	                    w_id = id2;
	                    l++;
	                }
	                if (w_id != u_id && w_id != v_id){
	                    card_union++;
	                    if (u_id < w_id && (v_id < w_id || -1 == get_neighbor_pos(edges, &u, w_id))){
	                        tritype = isoTricode(edges, &u, &v, w_id);
	                        triads[tritype]++;
	                    }
	                }
	            }
	            if (k>u.last_ind){
	                k = l;
	                z = v;
	                id = u_id;
	            } else {
	                z = u;
	                id = v_id;
	            }

	            while(k<=z.last_ind){
	                w_id = get_neighbor_id(edges[k]);
	                if (w_id != id){
	                    card_union++;
	                    if (u_id < w_id && (v_id < w_id || -1 == get_neighbor_pos(edges, &u, w_id))){
	                        tritype = isoTricode(edges, &u, &v, w_id);
	                        triads[tritype]++;
	                    }
	                }
	                k++;
	            }

	            if (BIDIRECTIONAL == get_direction(edges[j])){
	                tritype = 2;
	            } else {
	                tritype = 1;
	            }
	            triads[tritype] += num_nodes - card_union - 2;
	        }
        }
            
    }

	sum = 0;
	for (i=1; i<NUM_TRIADS;i++){
		sum += triads[i];
	}
	triads[0] = num_total_triads(num_nodes) - sum;
	return triads;
}





void print_triad_census(uint64_t* triads){
    int i;

    for(i=0;i<NUM_TRIADS;i++){
        if (triads[i]){
            std::cout << i+1 << " - " << triad_codes[i] << ": " << triads[i] << std::endl;
        }
    }
}





short isoTricode(EDGE_DEVICE* edges, NODE_DEVICE* u, NODE_DEVICE* v, uint32_t w_id){
	DIRECTION dir1, dir2, dir3;
 	int code;


 	dir1 = get_dir_between_nodes(edges,u, v->node_id);
 	dir2 = get_dir_between_nodes(edges,v, w_id);
 	dir3 = turnup(get_dir_between_nodes(edges,u, w_id));

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



int get_neighbor_pos(EDGE_DEVICE* edges, NODE_DEVICE* u, uint32_t v_id){
	return binary_search_adj_list(edges, v_id, u->first_ind, u->last_ind);
}

int binary_search_adj_list(EDGE_DEVICE* table, uint32_t key, int first, int last){
	int med;
	uint32_t med_id;
	while(first <= last){
		med = (first + last) / 2;
        med_id = get_neighbor_id(table[med]);
        if (med_id < key){
        	first = med+1;
        } else if (med_id > key){
        	last = med-1;
        } else {
        	return med;
        }
	}
	return -1;
}


DIRECTION get_dir_between_nodes(EDGE_DEVICE* edges, NODE_DEVICE* u, uint32_t v_id){
	int pos;
	pos = get_neighbor_pos(edges, u, v_id);
	if (-1 == pos){
		return NONE;
	}
	return get_direction(edges[pos]);
}



uint64_t num_total_triads(uint64_t num_nodes){
	return num_nodes*(num_nodes-1)*(num_nodes-2)/6;
}