#include <stdio.h>
#include <stdlib.h>


/**
  *   @file rand_graph_generation.c
  *
  *   @brief This program generates a pseudo-random graph of a given number of nodes and edges and writes it to a file.
  *   
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 28-12-2017
  **/

int main(int argc, char **argv){

	unsigned long n_nodes;
	unsigned long n_edges;
	int i;
	unsigned long u, v;
	FILE *fp;
	if(argc != 4){
		printf("Usage: %s -n_nodes -n_edges <outfile>\n", argv[0]);
		return EXIT_FAILURE;
	}
	fp = fopen(argv[3], "w");
	if (NULL == fp){
		printf("Error opening file %s\n", argv[3]);
		return EXIT_FAILURE;
	}

	n_nodes = (unsigned long) atol(argv[1]);
	n_edges = (unsigned long) atol(argv[2]);

	for(i=0;i<n_edges;i++){
		u = rand() % n_nodes;
		v = rand() % n_nodes;

		fprintf(fp, "%lu %lu\n", u, v);
	}

	fclose(fp);
	printf("Execution finished successfully\n");
	return EXIT_SUCCESS;
}
