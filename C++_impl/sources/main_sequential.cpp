#include "../headers/aux.h"

#include <string.h>
#include <getopt.h>
#include <sys/time.h>


#define MAX_CHAR 256
#define MAX_FUNC 5

void display_help(char *program);

int main(int argc, char **argv){
	GRAPH g;
	NODE_DEVICE* nodes;
	EDGE_DEVICE* edges;
	uint32_t num_nodes, num_edges;
	uint64_t *triads;

	struct timeval start, stop;   /*For counting elapsed execution time*/
	double reading_time, execution_time;
	FILE* fout;
	char input_name[MAX_CHAR], function_name[MAX_FUNC], times_name[MAX_CHAR];
	bool namefile, function, times, times_file, result, verbose;
	int long_index;
	char opt;	

		static struct option options[] = {
        {"graph",required_argument,0,'1'},
        {"function",required_argument,0,'2'},
        {"times",optional_argument,0,'3'},
        {"result", no_argument,0,'4'},
        {"verbose", no_argument,0,'5'},
        {"help", no_argument,0,'6'},
        {0,0,0,0}
    };


    namefile = function = times = times_file = result = verbose = false;
    long_index = 0;
    while ((opt = getopt_long_only(argc, argv,"1:2:3:456", options, &long_index)) != -1) {
        switch (opt) {
            case '1' :
            	if (strlen(optarg) > MAX_CHAR){
            		printf("The input file name cannot excede %d characters\n", MAX_CHAR);
            		return EXIT_FAILURE;
            	}
            	strcpy(input_name, optarg);
            	namefile = true;
                break;
            case '2' : 
                if (strcmp("BF", optarg) && strcmp("BM", optarg)){
					printf("\"%s\" is not a valid value for <function> parameters. Valid are \"BF\" or \"BM\"\n", optarg);
					return EXIT_FAILURE;
				}       
				strcpy(function_name, optarg);
            	function = true;  
                break;
            case '3' :
                if (optarg){
                	if (strlen(optarg) > MAX_CHAR){
                		printf("The times file name cannot excede %d characters\n", MAX_CHAR);
                		return EXIT_FAILURE;
                	}
                	strcpy(times_name, optarg);
                	times_file = true;
                } else {
                	times = true; 
                }     	   
                break;
            case '4':
            	result = true;
            	break;
            case '5':
            	verbose = true;
            	break;
            case '6':
            	display_help(argv[0]);
            	return EXIT_SUCCESS;
            case '?':
            default:
                display_help(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!namefile){
    	printf("You must specify an input file to read the graph\n");
    	display_help(argv[0]);
    	return EXIT_FAILURE;
    }
    if (!function){
    	printf("You must specify a function to perform the triadic census\n");
    	display_help(argv[0]);
    	return EXIT_FAILURE;
    }

	gettimeofday(&start, NULL);

	g = read_graph_from_file(input_name);

	gettimeofday(&stop, NULL);

	reading_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;

	if (verbose || times)
		printf("Graph read in %lf seconds.\n", reading_time);



	
	if (g.empty()){
		printf("Error creating graph!!\n");
		return EXIT_FAILURE;
	}


	if (ERR == convert_graph(g, &nodes, &edges, &num_nodes, &num_edges)){
		printf("Error converting graph\n");
		return EXIT_FAILURE;
	}

	if (verbose || times)
		display_graph_summary(g);

	if (get_num_nodes(g) <= 2){
		printf("Graph too small to perform triadic census\n");
		g.clear();
		return EXIT_SUCCESS;
	}
	

	
	gettimeofday(&start, NULL);

	if (!strcmp("BF", function_name))
		triads = triad_census_BF(nodes, edges, num_nodes);
	else
		triads = triad_census_BM(nodes, edges, num_nodes);


	gettimeofday(&stop, NULL);


	execution_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;

	if (verbose || times)
		printf("Triadic census performed in %lf seconds.\n", execution_time);

	if (verbose || result){
		printf("Triadic census:\n");
		print_triad_census(triads);
	}

	if (times_file){
		fout = fopen(times_name, "a+");
		if (!fout){
			printf("Error opening file %s\n", times_name);
			return EXIT_FAILURE;
		}
		fprintf(fout, "%" SCNu32 "\t%" SCNu32 "\t%lf\t%lf\n", get_num_nodes(g), get_num_edges(g), reading_time, execution_time);

		fclose(fout);
	}

	free(triads);
	g.clear();

	if(verbose)
		printf("Execution finished successfully\n");


	return EXIT_SUCCESS;

}


void display_help(char *program){
	printf("\nProgram to perform the triadic census of a graph.\n");
	printf("\nUse this program if you want to test correctness or performance of a triadic census algorithm on a given graph\n\n");
	printf("Usage: %s --graph <namefile> --function <function> [-t[=<timefile>]] [-r] [-v] [-o] [-h]\n", program);
	printf("Where:\n");
	printf("\t-g, --graph <namefile>: file containing input graph\n");
	printf("\t-f, --function <function>: name of the algorithm to apply. Valid values: \"BF\" or \"BM\"\n");
	printf("\t-t, --times [<timefile>]: Option to display times of execution\n");
	printf("\t-r, --result: Option to display results of triadic census\n");
	printf("\t-v, --verbose: Option to display info about execution process\n");
	printf("\t-o, --ordered: Option to save nodes and adjacencies in order (default is not-ordered)\n");
	printf("\t-h, --help: Option to display help\n");
}
