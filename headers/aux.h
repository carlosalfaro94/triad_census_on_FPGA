/**General helper functions**/
#include "../headers/triads.h"
#include <string.h>

void display_graph_summary(GRAPH* g);

BOOL is_ordered_kernel(char* name_kernel);

BOOL is_NDRange_kernel(char* name_kernel);

BOOL is_BM_kernel(char* name_kernel);

void write_times(char* times_name, GRAPH* g, double reading_time, double execution_time);

void rand_graph_generation(unsigned int num_nodes, unsigned int num_edges, unsigned int seed, char* namefile);