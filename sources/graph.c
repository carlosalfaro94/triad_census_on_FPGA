#include "../headers/graph.h"

BOOL ordered;   /*!< Flag to determine whether the data managed has to be ordered or not */


void set_ordered(BOOL value){
    ordered = value;
}

GRAPH* read_graph_from_file(char *filename){
    FILE *fp;
    GRAPH* g;
    uint32_t u,v;
    int scanerr;
    if (NULL == (fp = fopen(filename, "r"))){
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    if (NULL == (g = (GRAPH*) malloc (sizeof(GRAPH)))){
        printf("Error allocating memory for graph pointer\n");
        fclose(fp);
        return NULL;
    }

    g->num_nodes = 0;
    g->nodes = NULL;

    scanerr = 1;
    while (EOF != scanerr){
        //if (line[0] == '%'){
        //    continue;
        //}
        scanerr = fscanf(fp, "%" SCNu32 " %" SCNu32 "\n", &u, &v);
        if (2 != scanerr){
            scanerr = fscanf(fp, "%*s\n");
            continue;
        }
        if (u == v){
            continue;
        }

        if (!node_exists(g, u)){
            if (ERR == insert_node(g, u)){
                printf("Error Inserting node %u\n", u);
                return NULL;
            }
        }

        if (!node_exists(g,v)){
            if (ERR == insert_node(g, v)){
                printf("Error Inserting node %u\n", v);
                return NULL;
            }
        }


        if (ERR == add_edge(g,u,v,IN_OUT)){
            printf("Error adding edge %u %u IN_OUT\n", u,v);
            return NULL;
        }

        if (ERR == add_edge(g,v,u,OUT_IN)){
            printf("Error adding edge %u %u OUT_IN\n", u,v);
            return NULL;
        }
    }
    fclose(fp);
    return g;


}


STATUS insert_node(GRAPH* g, uint32_t node_id){
    NODE *n;
    //int i;
    if (NULL == g){
        printf("Error: Graph g is NULL\n");
        return ERR;
    }
    g->num_nodes++;
    if (NULL == (g->nodes = (NODE**) realloc (g->nodes, g->num_nodes*sizeof(NODE*)))){
        printf("Error while allocating a new slot for the node\n");
        return ERR;
    }

    if (NULL == (n = create_node(node_id))){
        printf("Error creating new node with id %u\n", node_id);
        return ERR;
    }

    /*Intern loop of insert sort*/

    if (ordered){
        insert_sort(g->nodes, sizeof(NODE*), g->num_nodes, (void*) &n, (void*) &node_id, &comp_nodes_by_id, &insert_nodes);
    } else {
        g->nodes[g->num_nodes-1] = n;
    }
    
    return OK;
}


STATUS add_edge(GRAPH* g, uint32_t node_id1, uint32_t node_id2, DIRECTION dir){
    int pos;
    if (NULL == g || !node_exists(g, node_id1)){
        printf("Error: g is null or %u is not in g\n", node_id1);
        return ERR;
    }
    pos = get_node_pos(g, node_id1);
    return add_edge_to_node(g->nodes[pos], node_id2, dir);
}


BOOL node_exists(GRAPH* g, uint32_t node_id){
    return (get_node_pos(g, node_id) != -1);
}


int get_node_pos(GRAPH* g, uint32_t node_id){
    if (NULL == g || NULL == g->nodes){
        return -1;
    }
    if (ordered){
        return binary_search((void*) g->nodes, 0, g->num_nodes-1, sizeof(NODE*), (void*) &node_id, &(comp_nodes_by_id));
    } else {
        return linear_search((void*) g->nodes, g->num_nodes, sizeof(NODE*), (void*) &node_id, &(comp_nodes_by_id));
    }
}


uint32_t get_num_nodes(const GRAPH* g){
    return g->num_nodes;
}

uint32_t get_num_pairs(const GRAPH* g){
    uint32_t n = g->num_nodes;
    return n*(n-1)/2;
}

uint64_t get_num_triples(const GRAPH* g){
    uint64_t n = (uint64_t) g->num_nodes;
    return n*(n-1)*(n-2)/6;
}


uint32_t get_num_edges(const GRAPH* g){
    uint32_t i;
    uint32_t num_edges;
    num_edges = 0;
    for (i=0;i<get_num_nodes(g);i++){
        num_edges+= get_degree(g->nodes[i]);
    }
    return num_edges / 2;
}


NODE* get_node_by_pos(const GRAPH* g, int pos){
    return g->nodes[pos];
}


NODE* get_node_by_id(GRAPH* g, uint32_t node_id){
    return get_node_by_pos(g, get_node_pos(g, node_id));
}


void print_graph(GRAPH* g){
    uint32_t i;
    for (i=0;i<g->num_nodes;i++){
        print_node(g->nodes[i]);
    }
}


void destroy_graph(GRAPH *g){
    uint32_t i;
    for (i = 0;i<g->num_nodes;i++){
        destroy_node(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}



STATUS convert_graph(GRAPH* g, NODE_DEVICE** nodes, EDGE_DEVICE** edges, uint32_t* num_nodes, uint32_t* num_edges){
    
    uint32_t i,j,k;
    uint32_t offset;
    uint32_t current_num_edges;
    NODE* n;
    *num_nodes = get_num_nodes(g);
    *num_edges = 0;
    for (i=0;i<(*num_nodes);i++){
        n = get_node_by_pos(g,i);
        (*num_edges)+=get_num_neighbors(n);
    }

    *nodes = (NODE_DEVICE*) malloc((*num_nodes)*sizeof(NODE_DEVICE));
    if (NULL == nodes){
        printf("Error creating memory for array of nodes\n");
        return ERR;
    }

    *edges = (EDGE_DEVICE*) malloc((*num_edges)*sizeof(EDGE_DEVICE));
    if (NULL == *edges){
        printf("Error creating memory for array of nodes\n");
        free(*nodes);
        return ERR;
    }
    offset = 0;
    for (i=0;i<(*num_nodes);i++){
        n = get_node_by_pos(g,i);
        (*nodes)[i].node_id = get_node_id(n);
        (*nodes)[i].first_ind = offset;
        current_num_edges = get_num_neighbors(n);
        if (current_num_edges > MAX_NEIGHBORS){
            printf("This software does not support graphs with such degrees.\n");
            return ERR;
        }
        offset+=current_num_edges;
        (*nodes)[i].last_ind = offset-1;
        for (j=0, k=(*nodes)[i].first_ind; j<current_num_edges;j++, k++){
            (*edges)[k] = create_edge(get_neighbor_id(n->adj_list[j]), get_direction(n->adj_list[j]));
        }

    }

    return OK;
}

TASK* create_tasks_array(GRAPH*g, NODE_DEVICE* nodes, uint32_t* num_tasks){
    uint32_t i, j, k;
    uint32_t u_id, v_id;
    TASK* tasks;
    NODE* u;
    (*num_tasks) = 0;
    tasks = NULL;

    for (i=0;i<get_num_nodes(g);i++){
        u = get_node_by_pos(g, i);
        u_id = get_node_id(u);
        for (j=0;j<get_num_neighbors(u);j++){
            v_id = get_neighbor_id(get_edge(u, j));
            if (u_id < v_id){
                (*num_tasks)++;
                tasks = (TASK*) realloc (tasks, (*num_tasks)*sizeof(TASK));
                tasks[(*num_tasks)-1].u = nodes[i];
                k = get_node_pos(g, v_id);
                tasks[(*num_tasks)-1].v = nodes[k];
            }
        }
    }

    return tasks;
}