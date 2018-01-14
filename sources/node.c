#include "../headers/node.h"

/**
  *   @file node.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file node.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 23-10-2017
  **/

extern BOOL ordered;   /*!< Flag to determine whether the data managed has to be ordered or not */

NODE* create_node(uint32_t node_id){
	NODE* n;
	if (NULL == (n = (NODE*) malloc(sizeof(NODE)))){
		printf("Error allocating memory for node\n");
		return NULL;
	}
	n->node_id = node_id;
	n->adj_list = NULL;
	n->num_neighbors=0;
	n->degree = 0;
	return n;
}


STATUS add_edge_to_node(NODE* node, uint32_t neighbor_id, DIRECTION dir){
	EDGE e;
	int pos;
	if (NULL == node){
		return ERR;
	}
	pos = get_neighbor_pos(node, neighbor_id);
	if (-1 != pos){
		if (get_direction(node->adj_list[pos])==dir || get_direction(node->adj_list[pos])==BIDIRECTIONAL)
			return OK;
		node->degree++;
		return set_direction(&(node->adj_list[pos]), dir);
	}
	node->degree++;
	node->num_neighbors++;

	if (NULL == (node->adj_list = (EDGE*)realloc(node->adj_list, node->num_neighbors*sizeof(EDGE)))){
		printf("Error allocating memory for new edge\n");
		return ERR;
	}
	e = create_edge(neighbor_id,dir);

	if (ordered){
		insert_sort((void*) node->adj_list, sizeof(EDGE), node->num_neighbors, (void*) &e, (void*) &neighbor_id, &comp_edges, &insert_edge);
	} else {
		node->adj_list[node->num_neighbors-1] = e;
	}

	

    return OK;
}	


int get_neighbor_pos(NODE* n, uint32_t neighbor_id){
	if (ordered)
		return binary_search((void*) n->adj_list, 0, n->num_neighbors-1, sizeof(EDGE), (void*) &neighbor_id, &(comp_edges));
	else 
		return linear_search((void*) n->adj_list, n->num_neighbors, sizeof(EDGE), (void*) &neighbor_id, &(comp_edges));
}


BOOL not_connected(NODE* u, NODE* v){
	return (-1 == get_neighbor_pos(u, get_node_id(v)));
}


uint32_t get_node_id(NODE* n){
	return n->node_id;
}

void set_node_id(NODE* n, uint32_t id){
	n->node_id = id;
}

EDGE get_edge(NODE* n, int pos){
	return n->adj_list[pos];
}

void set_edge(NODE* n, int pos, EDGE e){
	n->adj_list[pos] = e;
}


EDGE* get_adj_list(NODE* n){
	return n->adj_list;
}


int get_num_neighbors(NODE* n){
	return n->num_neighbors;
}

int get_degree(NODE* n){
	return n->degree;
}


int comp_nodes_by_id(const void* key, const void* pnode){
    return (int) *((uint32_t*) key) - (int) (*((NODE**) pnode))->node_id;
}

int comp_nodes_by_degree(const void* pnode1, const void* pnode2){
	return (int) (*((NODE**) pnode1))->num_neighbors - (int) (*((NODE**) pnode2))->num_neighbors;
}

int comp_nodes_by_degree_rev(const void* pnode1, const void* pnode2){
	return (int) (*((NODE**) pnode2))->num_neighbors - (int) (*((NODE**) pnode1))->num_neighbors;
}

int comp_ids(const void* key, const void* pid){
	return (int) *((uint32_t*) key) - (int) *((uint32_t*) pid);
}

void insert_nodes(void* nodes, void* node, int pos){
	((NODE**) nodes)[pos] = (*((NODE**) node));
}


uint32_t* union_adj_lists_ordered(int* card_S, NODE* u, NODE* v){
	int i, j;
	uint32_t id1, id2, id;
	uint32_t* S = NULL;

	if (NULL == u || NULL == v){
		printf("Must pass not null pointers!\n");
		return NULL;
	}

	*card_S = 0;
	if (NULL == u->adj_list && NULL == v->adj_list){
		return NULL;
	}
	if (NULL == u->adj_list){
		*card_S = v->num_neighbors;
		S = (uint32_t*) malloc(v->num_neighbors*sizeof(uint32_t));
		for (i=0;i<v->num_neighbors;i++){
			S[i] = get_neighbor_id(v->adj_list[i]);
		}
		return S;
	}
	if (NULL == v->adj_list){
		*card_S = u->num_neighbors;
		S = (uint32_t*) malloc(u->num_neighbors*sizeof(uint32_t));
		for (i=0;i<u->num_neighbors;i++){
			S[i] = get_neighbor_id(u->adj_list[i]);
		}
		return S;
	}
	i=0;j=0;
	while(i<get_num_neighbors(u) && j<get_num_neighbors(v)){
		id1 = get_neighbor_id(u->adj_list[i]);
		id2 = get_neighbor_id(v->adj_list[j]);
		if (id1 == id2){
			id = id1;
			i++;j++;
		} else if (id1 < id2){
			id = id1;
			i++;
		} else {
			id = id2;
			j++;
		}
		if (id != get_node_id(u) && id != get_node_id(v)){
			(*card_S)++;
			S = (EDGE*) realloc (S, (*card_S)*sizeof(EDGE));
			S[(*card_S)-1] = id;
		}
	}
	while (i<get_num_neighbors(u)){
		id = get_neighbor_id(u->adj_list[i]);
		if (id != get_node_id(v)){
			(*card_S)++;
			S = (EDGE*) realloc (S, (*card_S)*sizeof(EDGE));
			S[(*card_S)-1] = id;
		}
		i++;
	} 
	while (j<get_num_neighbors(v)){
		id = get_neighbor_id(v->adj_list[j]);
		if (id != get_node_id(u)){
			(*card_S)++;
			S = (EDGE*) realloc (S, (*card_S)*sizeof(EDGE));
			S[(*card_S)-1] = id;
		}
		j++;
	}

	return S;
}

uint32_t* union_adj_lists_not_ordered(int* card_S, NODE* u, NODE* v){
	int i, j;
	uint32_t id;
	uint32_t* S = NULL;

	*card_S = 0;
	for (i=0;i<u->num_neighbors;i++){
		id = get_neighbor_id(u->adj_list[i]);
		if (id != get_node_id(v)){
			(*card_S)++;
			S = (EDGE*) realloc (S, (*card_S)*sizeof(EDGE));
			S[(*card_S)-1] = id;
		}
	}
	for (i=0;i<v->num_neighbors;i++){
		id = get_neighbor_id(v->adj_list[i]);
		j = linear_search((void*) S, *card_S, sizeof(uint32_t), (void*) &id, &(comp_ids));
		if (j != -1)
			continue;
		if (id != get_node_id(u)){
			(*card_S)++;
			S = (EDGE*) realloc (S, (*card_S)*sizeof(EDGE));
			S[(*card_S)-1] = id;
		}
	}

	return S;
}

DIRECTION get_dir_between_nodes(NODE* u, NODE* v){
	if (not_connected(u,v)){
		return NONE;
	}
	return get_direction(get_edge(u, get_neighbor_pos(u, get_node_id(v))));
}


void print_node(NODE* n){
	int i;
	for (i=0;i<n->num_neighbors;i++){
		print_edge(n->node_id, n->adj_list[i]);
	}
	printf("\n");
}


void destroy_node(NODE *n){
	free(n->adj_list);
	free(n);
}