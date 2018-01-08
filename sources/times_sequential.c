/*Main de pruebas*/

#include "../headers/aux.h"
#include <string.h>
#include <getopt.h>
#include <sys/time.h>


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
	BOOL namefile, function, times, times_file, result, verbose;
	int long_index;
	char opt;
	unsigned int n_nodes, n_edges;
	unsigned int num_iter;
	int k;


	static struct option options[] = {
        {"graph",required_argument,0,'1'},
        {"function",required_argument,0,'2'},
        {"times",required_argument,0,'3'},
        {"nodes", required_argument,0,'4'},
        {"edges", required_argument,0,'5'},
        {"iterations", required_argument,0,'6'},
        {"ordered", no_argument,0,'7'},
        {"help", no_argument,0,'8'},
        {0,0,0,0}
    };


    namefile = function = times = times_file = result = verbose = FALSE;
    long_index = 0;
    set_ordered(FALSE);
    while ((opt = getopt_long_only(argc, argv,"1:2:3:4:5:6:7:8", options, &long_index)) != -1) {
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
                if (strlen(optarg) > MAX_CHAR){
                    printf("The times file name cannot excede %d characters\n", MAX_CHAR);
                    return EXIT_FAILURE;
                }
                strcpy(times_name, optarg);
                times_file = TRUE;   	   
                break;
            case '4':
            	if (atoi(optarg) <= 0){
            		printf("Number of nodes must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                n_nodes = atoi(optarg);
                break;
            case '5':
                if (atoi(optarg) <= 0){
            		printf("Number of edges must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                n_edges = atoi(optarg);
                break;
            case '6':
                if (atoi(optarg) <= 0){
            		printf("Number of iterations must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                num_iter = atoi(optarg);
                break;
            case '7':
            	set_ordered(TRUE);
            	break;
            case '8':
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

    for (k=0; k<num_iter;k++){

    	rand_graph_generation(n_nodes, n_edges, k, input_name);

		gettimeofday(&start, NULL);

		g = read_graph_from_file(input_name);

		gettimeofday(&stop, NULL);

		reading_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;

		//if (verbose || times)
		//	printf("Graph read in %lu microseconds.\n", reading_time);
		
		if (NULL == g){
			printf("Error creating graph!!\n");
			return EXIT_FAILURE;
		}


		if (get_num_nodes(g) <= 2){
			printf("Graph too small to perform triadic census\n");
			destroy_graph(g);
			return EXIT_SUCCESS;
		}
		
		gettimeofday(&start, NULL);

		if (!strcmp("BF", function_name))
			triads = triad_census_BF(g);
		else
			triads = triad_census_BM(g);

		gettimeofday(&stop, NULL);


		execution_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;

		
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
	}

	return EXIT_SUCCESS;

}


void display_help(char *program){
	printf("\nProgram to generate sequential triad census times\n");
	printf("\nUse this program if you want to generate multiple graphs and collect execution times to estimate the performance of the algorithm\n\n");
	printf("Usage: %s -g <namefile> -f <function> -t <timefile> -n <num_nodes> -e <num_edges> -i <num_iter> [-o] [-h]\n", program);
	printf("Where:\n");
	printf("\t-g, --graph <namefile>: file to save generated graphs\n");
	printf("\t-f, --function <function>: name of the algorithm to apply. Valid values: \"BF\" or \"BM\"\n");
	printf("\t-t, --times <timefile>: File to save times of execution\n");
	printf("\t-n, --nodes <num_nodes>: Number of nodes of generated graphs\n");
	printf("\t-e, --edges <num_edges>: Number of edges of generated graphs\n");
	printf("\t-i, --iterations <num_iter>: Number of graphs generated\n");
	printf("\t-o, --ordered: Option to save nodes and adjacencies in order (default is not-ordered)\n");
	printf("\t-h, --help: Option to display help\n");
}

