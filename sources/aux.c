#include "../headers/aux.h"

/**
  *   @file aux.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file aux.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 18-12-2017
  **/

void display_graph_summary(GRAPH* g){
	printf("Graph summary\n");
    printf("\t - Number of nodes: %" SCNu32 "\n", get_num_nodes(g));
    printf("\t - Number of edges: %" SCNu32 "\n", get_num_edges(g));
    printf("\t - Number of triads: %" SCNu64 "\n", num_total_triads(get_num_nodes(g)));
}

BOOL is_ordered_kernel(char* name_kernel){
    return (NULL != strstr(name_kernel, "ord"));
}

BOOL is_NDRange_kernel(char* name_kernel){
    return (NULL != strstr(name_kernel, "NDRange"));
}

BOOL is_BM_kernel(char* name_kernel){
    return (NULL != strstr(name_kernel, "BM"));
}

void write_times(char* times_name, GRAPH* g, double reading_time, double execution_time){
	FILE* fout;
	fout = fopen(times_name, "a+");
    if (!fout){
        printf("Error opening file %s\n", times_name);
        exit(EXIT_FAILURE);
    }
    fprintf(fout, "%" SCNu32 "\t%" SCNu32 "\t%lf\t%lf\n", get_num_nodes(g), get_num_edges(g), reading_time, execution_time);
    fclose(fout);
}



void rand_graph_generation(unsigned int num_nodes, unsigned int num_edges, unsigned int seed, char* namefile){
	FILE* fp;
	unsigned int u, v;
	int i;
	fp = fopen(namefile, "w");
	if (NULL == fp){
		printf("Error opening file %s\n", namefile);
		return;
	}


	srand(seed);
	for(i=0;i<num_edges;i++){
		u = rand() % num_nodes;
		v = rand() % num_nodes;

		fprintf(fp, "%u %u\n", u, v);
	}

	fclose(fp);
}