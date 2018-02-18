#include "../headers/triads.h"


/**
  *   @file triads.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file triads.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

extern BOOL ordered; /*!< Flag to determine whether the data managed has to be ordered or not */
const char* triad_codes[NUM_TRIADS] = {"003","012","102","021D","021U","021C","111D","111U","030T","030C","201","120D","120U","120C","210","300"};  /*!< Triad codes */

uint64_t* triad_census_BF(GRAPH* g){
	uint64_t* triads;
	int i, j, k;
	int tritype;
	uint32_t num_nodes;
	NODE *u, *v, *w;

	num_nodes = get_num_nodes(g);

	triads = (uint64_t*) calloc(NUM_TRIADS, sizeof(uint64_t));

	if (NULL == triads){
		printf("Error allocating memory for triad census array\n");
		return NULL;
	}

	for (i=0; i<num_nodes; i++){
		for (j=i+1;j<num_nodes;j++){
			for(k=j+1;k<num_nodes;k++){
				u = get_node_by_pos(g, i);
				v = get_node_by_pos(g, j);
				w = get_node_by_pos(g, k);
				tritype = isoTricode(u,v,w);
				triads[tritype]++;
			}
		}
	}

	return triads;

}


uint64_t* triad_census_BM(GRAPH* g){
	uint64_t* triads;
	int i, j, k;
	int tritype;
	uint32_t num_nodes, num_adjs;
	NODE *u, *v, *w;
	EDGE *N_u;
	uint32_t* S;
	int card_S;
	uint32_t u_id, v_id, w_id;
	uint64_t sum;

	num_nodes = get_num_nodes(g);
	S = NULL;
	triads = (uint64_t*) calloc(NUM_TRIADS, sizeof(uint64_t));

	if (NULL == triads){
		printf("Error allocating memory for triad census array\n");
		return NULL;
	}

	for (i=0; i<num_nodes; i++){
		u = get_node_by_pos(g,i);
		u_id = get_node_id(u);
		N_u = get_adj_list(u);
		num_adjs = get_num_neighbors(u);
		for (j = 0; j < num_adjs; j++){
			v_id = get_neighbor_id(N_u[j]);
			if (u_id < v_id){
				v = get_node_by_id(g, v_id);
				if (ordered)
					S = union_adj_lists_ordered(&card_S, u, v);
				else 
					S = union_adj_lists_not_ordered(&card_S, u, v);
				if (BIDIRECTIONAL == get_direction(N_u[j])){
					tritype = 2;
				} else {
					tritype = 1;
				}
				triads[tritype] += num_nodes - card_S - 2; 
				for (k=0;k<card_S;k++){
					w_id = S[k];
					w = get_node_by_id(g, w_id);
					if (v_id < w_id || (u_id < w_id && w_id < v_id && not_connected(u,w))){
						tritype = isoTricode(u,v,w);
						triads[tritype] += 1;
					}
				}
				free(S);
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
			printf("%d - %s: %" SCNu64 "\n", i+1, triad_codes[i], triads[i]);
		}
	}
}

#if 0
void print_triad_census_64b(uint64_t* triads){
	int i;

	for(i=0;i<NUM_TRIADS;i++){
		if (triads[i]){
			printf("%d - %s: %" SCNu64 "\n", i+1, triad_codes[i], triads[i]);
		}
	}
}
#endif


const char* return_code(int* triads){
	int i;
	for (i=0;i<NUM_TRIADS;i++){
		if(triads[i])
			return triad_codes[i];
	}
	return NULL;
}


int isoTricode(NODE* u, NODE* v, NODE* w){
	DIRECTION dir1, dir2, dir3;
 	int code;

	dir1 = get_dir_between_nodes(u,v);
	dir2 = get_dir_between_nodes(v,w);
	dir3 = get_dir_between_nodes(w,u);

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


uint64_t num_total_triads (uint32_t n){
	return (uint64_t) n*(n-1)*(n-2) / 6;
}



