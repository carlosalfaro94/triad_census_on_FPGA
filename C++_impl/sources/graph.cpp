
#include "../headers/graph.h"

GRAPH read_graph_from_file(char* filename){
	GRAPH g;
	FILE *fp;
	uint32_t u,v;
    int scanerr;
    uint32_t u_id, v_id, current_node_id;
    std::map<uint32_t, uint32_t> node2id;
    std::map<uint32_t, uint32_t>::iterator node2id_it, node2id_end;

	fp = fopen(filename, "r");
	if (NULL == fp){
		std::cerr << "Can't open file " << filename << std::endl;
		std::exit(1);
	}

    current_node_id = 0;
	while (EOF != scanerr){
		scanerr = fscanf(fp, "%" SCNu32 " %" SCNu32 "\n", &u, &v);
		if (2 != scanerr){
            scanerr = fscanf(fp, "%*s\n");
            continue;
        }
        if (u == v){
            continue;
        }
        node2id_it = node2id.find(u);
        node2id_end = node2id.end();
        if (node2id_it == node2id_end){
            u_id = current_node_id;
            node2id[u] = u_id;
            current_node_id++;
        } else {
            u_id = node2id_it->second;
        }

        node2id_it = node2id.find(v);
        if (node2id_it == node2id_end){
            v_id = current_node_id;
            node2id[v] = v_id;
            current_node_id++;
        } else {
            v_id = node2id_it->second;
        }


        EDGE e1 = create_edge(v_id, IN_OUT);
        EDGE e2 = create_edge(u_id, OUT_IN);
        LIST_EDGES::iterator it1 = g[u_id].find(e1);
        LIST_EDGES::iterator it2 = g[v_id].find(e2);

        if (g[u_id].end() != it1 && OUT_IN == get_direction(*it1)){  /*If the edge already exists in the oposite direction...*/
        	g[u_id].erase(it1);
        	g[u_id].insert(create_edge(v_id, BIDIRECTIONAL));   /*Create bidirectional edge*/
        } else {
        	g[u_id].insert(e1);
        }


        if (g[v_id].end() != it2 && IN_OUT == get_direction(*it2)){   /*Same for node v*/
    		g[v_id].erase(it2);
    		g[v_id].insert(create_edge(u_id, BIDIRECTIONAL));
        } else {
        	g[v_id].insert(e2);
        }

	}
    node2id.clear();
	return g;
}


uint32_t get_num_nodes(GRAPH g){
    return g.size();
}

uint32_t get_num_edges(GRAPH g){
    uint32_t num_edges;
    num_edges = 0;
    for (const auto &it: g){
        num_edges+=it.second.size();
    }
    return num_edges;
}

uint32_t get_max_degree(GRAPH g){
    uint32_t current, max;
    max = 1;
    for (const auto &it: g){
        current = it.second.size();
        max = (max < current)? current : max;
    }
    return max;
}


STATUS convert_graph(GRAPH g, NODE_DEVICE** nodes, EDGE_DEVICE** edges, uint32_t* num_nodes, uint32_t* num_edges){
    
    uint32_t i,j;
    uint32_t offset;
    uint32_t current_num_edges;

    *num_nodes = get_num_nodes(g);
    *num_edges = get_num_edges(g);

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
    i=0;
    for (const auto &it: g){
        (*nodes)[i].node_id = it.first;
        (*nodes)[i].first_ind = offset;
        current_num_edges = it.second.size();
        if (current_num_edges > MAX_NEIGHBORS){
            printf("This software does not support graphs with such degrees.\n");
            free(*nodes);
            free(*edges);
            return ERR;
        }
        offset+=current_num_edges;
        (*nodes)[i].last_ind = offset-1;
        j=(*nodes)[i].first_ind;
        for (const auto &it2 : it.second){
            (*edges)[j] = it2;
            j++;
        }
        i++;
    }

    return OK;
}


TASK* create_tasks_array(GRAPH g, NODE_DEVICE* nodes, uint32_t* num_tasks){
    TASK* tasks;
    *num_tasks = 0;
    uint32_t u_id, v_id;
    uint32_t i;
    for (const auto &node: g){
        u_id = node.first;
        for (const auto &edge: node.second){
            v_id = get_neighbor_id(edge);
            if (u_id < v_id){
                (*num_tasks)++;
            }
        }
    }
    tasks = (TASK*) calloc (*num_tasks, sizeof(TASK));
    if (NULL == tasks){
        printf("Error allocating memory for tasks array\n");
        return NULL;
    }
    i=0;
    for (const auto &node: g){
        u_id = node.first;
        for (const auto &edge: node.second){
            v_id = get_neighbor_id(edge);
            if (u_id < v_id){
                tasks[i].u = nodes[u_id];
                tasks[i].v = nodes[v_id];
                i++;
            }
        }
    }
    return tasks;
}




void print_graph(GRAPH g){
    GRAPH::iterator it1;
    LIST_EDGES::iterator it2;
    LIST_EDGES edges;
    for (it1 = g.begin(); it1 != g.end(); ++it1){
        edges = it1->second;
        for (it2 = edges.begin();it2!=edges.end();++it2){
            print_edge(it1->first, *it2);
        }
        std::cout << std::endl;
    }
}