#include "../headers/aux_opencl.h"

#include <getopt.h>
#include <sys/time.h>


/**
  *   @file times_parallel.c
  *
  *   @brief This program collects execution performances of a user-specified accelerated version of the triad census algorithm
  *   and writes them to a file
  *   
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 28-12-2017
  **/


void display_help(char *program);

int main(int argc, char **argv){
    
    GRAPH* g;
    NODE_DEVICE* nodes_host;
    EDGE_DEVICE* edges_host;
    TASK* tasks_host;
    CENSUS* census_host_NDRange;
    cl_uint num_nodes, num_edges, num_tasks, num_works;
    unsigned int n_nodes, n_edges;
    cl_ulong* census_host;

    cl_int err;  /*To control errs*/
    cl_platform_id platform;  /*Variables to set up environment*/
    cl_device_id device;
    cl_context context;
    cl_mem nodes_device, edges_device, tasks_device, census_device;
    cl_command_queue queue;  
    cl_program program;
    cl_kernel kernel;
    cl_event e;   /*Event arrays for synchronization*/
    struct timeval start, stop;   /*For counting elapsed execution time*/
    //clock_t start, stop, start_1, stop_1;
    //time_t reading_time, execution_time, reduction_time;
    double reading_time, execution_time;

    char input_name[MAX_CHAR], input_binary[MAX_CHAR], name_kernel[MAX_CHAR], times_name[MAX_CHAR];
    BOOL namefile, binary, function, times, times_file, result, verbose, ordered;
    int long_index;
    char opt;
    int i, j, k;
    unsigned int num_iter;

    static struct option options[] = {
        {"graph",required_argument,0,'1'},
        {"binary",required_argument,0,'2'},
        {"function",required_argument,0,'3'},
        {"times",required_argument,0,'4'},
        {"nodes", required_argument,0,'5'},
        {"edges", required_argument,0,'6'},
        {"iterations", required_argument,0,'7'},
        {"ordered", no_argument,0,'8'},
        {"help", no_argument,0,'9'},
        {0,0,0,0}
    };


    namefile = binary = function = times = times_file = result = verbose = ordered = FALSE;
    long_index = 0;
    while ((opt = getopt_long_only(argc, argv,"1:2:3:4:5:6:7:8:9", options, &long_index)) != -1) {
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
                if (strlen(optarg) > MAX_CHAR){
                    printf("The input file name cannot excede %d characters\n", MAX_CHAR);
                    return EXIT_FAILURE;
                }
                strcpy(input_binary, optarg);
                binary = TRUE;
                break;
            case '3' :   
                strcpy(name_kernel, optarg);
                function = TRUE;  
                break;
            case '4' :
                if (strlen(optarg) > MAX_CHAR){
                    printf("The times file name cannot excede %d characters\n", MAX_CHAR);
                    return EXIT_FAILURE;
                }
                strcpy(times_name, optarg);
                times_file = TRUE;     
                break;
            case '5':
            	if (atoi(optarg) <= 0){
            		printf("Number of nodes must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                n_nodes = atoi(optarg);
                break;
            case '6':
                if (atoi(optarg) <= 0){
            		printf("Number of edges must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                n_edges = atoi(optarg);
                break;
            case '7':
                if (atoi(optarg) <= 0){
            		printf("Number of iterations must be a positive integer\n");
            		return EXIT_FAILURE;
            	}
                num_iter = atoi(optarg);
                break;
            case '8':
            	set_ordered(TRUE);
            	ordered = TRUE;
            	break;
            case '9':
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
    if(!binary){
        printf("You must specify input binary file that contains kernel functions\n");
        display_help(argv[0]);
        return EXIT_FAILURE;
    }
    if (!function){
        printf("You must specify a function to perform the triadic census\n");
        display_help(argv[0]);
        return EXIT_FAILURE;
    }

    if (!times_file){
        printf("You must specify a file to save the execution times\n");
        display_help(argv[0]);
        return EXIT_FAILURE;
    }

    if (ordered && !(is_ordered_kernel(name_kernel))){
        printf("%s is a kernel that expects not ordered input...\n", name_kernel);
        return EXIT_FAILURE;
    }
    if (!ordered && is_ordered_kernel(name_kernel)){
        printf("%s is a kernel that expects ordered input...\n", name_kernel);
        return EXIT_FAILURE;
    }


    //set up execution environment for kernels
    setup(&platform, &device, &context);
    if (verbose){
        display_platform_info(platform);
        display_device_info(device);
    }
    queue = clCreateCommandQueue(context, device, 0, &err);
    if (NULL == (program = create_build_program(input_binary, context, &device))){
        printf("An error ocurred while creating the program\n");
        return EXIT_FAILURE;
    }
    kernel = clCreateKernel(program, name_kernel, &err);
    check_error(err, "Error creating kernel.");


    for (k=0;k<num_iter;k++){
    	rand_graph_generation(n_nodes, n_edges, k, input_name);
    	gettimeofday(&start, NULL);
	    //start = clock();

	    g = read_graph_from_file(input_name);

	    gettimeofday(&stop, NULL);
	    //stop = clock();

	    reading_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;
	    //reading_time = ((double) (stop - start)) / CLOCKS_PER_SEC;

	    
	    if (NULL == g){
	        printf("Error creating graph!!\n");
	        return EXIT_FAILURE;
	    }


	    if (get_num_nodes(g) <=2){
	        printf("Graph is too small to perform triadic census\n");
	        destroy_graph(g);
	        return EXIT_SUCCESS;
	    }

	    if (ERR == convert_graph(g, &nodes_host, &edges_host, &num_nodes, &num_edges)){
	        printf("Error corverting graph!!");
	        return EXIT_FAILURE;
	    }

	    if (NULL == (census_host = (cl_ulong*) calloc (NUM_TRIADS,sizeof(cl_ulong)))){
	        printf("Error while allocating memory for census array\n");
	        return EXIT_FAILURE;
	    }


	    /*CREATE DEVICE BUFFERS*/
	    if (is_NDRange_kernel(name_kernel) && is_BM_kernel(name_kernel)){
	        tasks_host = create_tasks_array(g, nodes_host, &num_tasks);
	        tasks_device = create_and_write_buffer(context, queue, num_tasks*sizeof(TASK), tasks_host);
	        num_works = num_tasks;
	    } else {
	    	nodes_device = create_and_write_buffer(context, queue, num_nodes*sizeof(NODE_DEVICE), nodes_host);
	        num_works = num_nodes;
	    }

	    edges_device = create_and_write_buffer(context, queue, num_edges*sizeof(EDGE_DEVICE), edges_host);

	    if (is_NDRange_kernel(name_kernel)){
	        census_host_NDRange = (CENSUS*) calloc(num_works, sizeof(CENSUS));
	        if (!census_host_NDRange){
	            printf("Error allocating memory for NDRange census.\n");
	            return EXIT_FAILURE;
	        }
	        census_device = create_and_write_buffer(context, queue, num_works*sizeof(CENSUS), census_host_NDRange);
	    } else {
	    	census_device = create_and_write_buffer(context, queue, NUM_TRIADS*sizeof(cl_ulong), census_host);
	    }


	    /*SET KERNEL PARAMS*/
	    if (is_NDRange_kernel(name_kernel) && is_BM_kernel(name_kernel)){
	    	set_args(kernel, (void *) &tasks_device, (void *) &edges_device, (void *) &census_device, (void *) &num_nodes);
	    } else {
	    	set_args(kernel, (void *) &nodes_device, (void *) &edges_device, (void *) &census_device, (void *) &num_nodes);
	    }

	    gettimeofday(&start, NULL);
	    //start = clock();
	    if (is_NDRange_kernel(name_kernel)){
	        const size_t globalWorkSize = num_works;
	        err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, &globalWorkSize, NULL, 0, NULL, &e);
	        check_error(err, "error executing NDRange kernel.");
	        err = clEnqueueReadBuffer(queue, census_device, CL_TRUE, 0, num_works*sizeof(CENSUS), census_host_NDRange, 1, &e, NULL);
	        check_error(err, "error reading buffer.");
	        for(i=0;i<num_works;i++){
	            for (j=0;j<NUM_TRIADS;j++){
	                census_host[j]+=census_host_NDRange[i].census[j];
	            }
	        }
	    } else {
	        clEnqueueTask(queue, kernel, 0, NULL, &e);
	        err = clEnqueueReadBuffer(queue, census_device, CL_TRUE, 0, NUM_TRIADS*sizeof(cl_ulong), census_host, 1, &e, NULL);
	    } 
	    gettimeofday(&stop, NULL);
	    //stop = clock();
	    execution_time = (stop.tv_sec - start.tv_sec) +  ((double) (stop.tv_usec - start.tv_usec))/1000000;
	    //execution_time = ((double) (stop - start)) / CLOCKS_PER_SEC;

	    if (is_BM_kernel(name_kernel)){
	        uint64_t sum = 0;
	        for(j=1;j<NUM_TRIADS;j++){
	            sum+= census_host[j];
	        }
	        census_host[0] = num_total_triads(get_num_nodes(g)) - sum;
	    }

	    if (times_file){
	    	write_times(times_name, g, reading_time, execution_time);
	    }
	    

	    
	    clReleaseMemObject(nodes_device); clReleaseMemObject(edges_device); clReleaseMemObject(census_device);
	    
	    clReleaseEvent(e);


	    free(nodes_host);
	    free(edges_host);
	    free(census_host);

	    destroy_graph(g);
    }


    

    clReleaseContext(context);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseKernel(kernel);

    return EXIT_SUCCESS;

}


void display_help(char *program){
    printf("\nProgram to generate accelerated triad census times\n");
    printf("\nUse this program if you want to generate multiple graphs and collect execution times to estimate the performance of the algorithm\n\n");
    printf("Usage: %s -g <namefile> -b <aocx file> -f <kernel_name> -t <timefile> -n <num_nodes> -e <num_edges> -i <num_iter> [-o] [-h]\n", program);
    printf("Where:\n");
    printf("\t-g, --graph <namefile>: file containing input graph\n");
    printf("\t-b, --binary <aocx file>: aocx file containing bitstream for kernel functions\n");
    printf("\t-f, --function <kernel_name>: name of the kernel synthetized on the aocx file\n");
    printf("\t-t, --times <timefile>: File to save times of execution\n");
    printf("\t-n, --nodes <num_nodes>: Number of nodes of generated graphs\n");
    printf("\t-e, --edges <num_edges>: Number of edges of generated graphs\n");
    printf("\t-i, --iterations <num_iter>: Number of graphs generated\n");
    printf("\t-o, --ordered: Option to save nodes and adjacencies in order (default is not-ordered)\n");
    printf("\t-h, --help: Option to display help\n");
}
