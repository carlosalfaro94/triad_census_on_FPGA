#include "../headers/aux.h"

#include <string.h>
#include <getopt.h>
#include <sys/time.h>


/**
  *   @file main_sequential.c
  *
  *   @brief This is the main program of the sequential application. It executes a user-specified sequential
  *   version of triad census algorithm and displays results and execution performances depending on the parameters 
  *   passed to the program.
  *   
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 28-12-2017
  **/

#define MAX_CHAR 256
#define MAX_FUNC 5



void display_help(char *program);


int main(int argc, char **argv){
	GRAPH* g;
	uint64_t *triads;
	struct timeval start, stop;   /*For counting elapsed execution time*/
	double reading_time, execution_time;
	FILE* fout;
	char input_name[MAX_CHAR], function_name[MAX_FUNC], times_name[MAX_CHAR];
	BOOL namefile, function, times, times_file, result, verbose, ord_deg, ord_deg_rev;
	int long_index;
	char opt;



	static struct option options[] = {
        {"graph",required_argument,0,'1'},
        {"function",required_argument,0,'2'},
        {"times",optional_argument,0,'3'},
        {"result", no_argument,0,'4'},
        {"verbose", no_argument,0,'5'},
        {"ordered", optional_argument,0,'6'},
        {"help", no_argument,0,'7'},
        {0,0,0,0}
    };


    namefile = function = times = times_file = result = verbose = ord_deg = ord_deg_rev = FALSE;
    long_index = 0;
    set_ordered(FALSE);
    while ((opt = getopt_long_only(argc, argv,"1:2:3:4567", options, &long_index)) != -1) {
        switch (opt) {
            case '1' :
            	if (strlen(optarg) > MAX_CHAR){
            		printf("The input file name cannot excede %d characters\n", MAX_CHAR);
            		return EXIT_FAILURE;
            	}
            	strcpy(input_name, optarg);
            	namefile = TRUE;
                break;
            case '2' : 
                if (strcmp("BF", optarg) && strcmp("BM", optarg)){
					printf("\"%s\" is not a valid value for <function> parameters. Valid are \"BF\" or \"BM\"\n", optarg);
					return EXIT_FAILURE;
				}       
				strcpy(function_name, optarg);
            	function = TRUE;  
                break;
            case '3' :
                if (optarg){
                	if (strlen(optarg) > MAX_CHAR){
                		printf("The times file name cannot excede %d characters\n", MAX_CHAR);
                		return EXIT_FAILURE;
                	}
                	strcpy(times_name, optarg);
                	times_file = TRUE;
                } else {
                	times = TRUE; 
                }     	   
                break;
            case '4':
            	result = TRUE;
            	break;
            case '5':
            	verbose = TRUE;
            	break;
            case '6':
            	set_ordered(TRUE);
            	if(optarg){
                    if (!strcmp(optarg, "deg")){
                        ord_deg = TRUE;
                    } else if (!strcmp(optarg, "deg-r")){
                        ord_deg_rev = TRUE;
                    } else {
                        printf("Error in argument -o\n");
                        display_help(argv[0]);
                        return EXIT_FAILURE;
                    }
                }
            	break;
            case '7':
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
	
	if (NULL == g){
		printf("Error creating graph!!\n");
		return EXIT_FAILURE;
	}

	if (get_num_nodes(g) <= 2){
		printf("Graph too small to perform triadic census\n");
		destroy_graph(g);
		return EXIT_SUCCESS;
	}

	if (verbose){
		display_graph_summary(g);
	}
	if (ord_deg){
        qsort(g->nodes, get_num_nodes(g), sizeof(NODE*), &comp_nodes_by_degree);
    } else if (ord_deg_rev){
        qsort(g->nodes, get_num_nodes(g), sizeof(NODE*), &comp_nodes_by_degree_rev);
    }

	//printf("Performing triadic census with the %s algorithm...\n", argv[2]);
	
	gettimeofday(&start, NULL);

	if (!strcmp("BF", function_name))
		triads = triad_census_BF(g);
	else
		triads = triad_census_BM(g);


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

	destroy_graph(g);


	if (verbose)
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
