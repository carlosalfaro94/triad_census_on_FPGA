

#include "../headers/aux_opencl.h"

#include <getopt.h>
#include <sys/time.h>


void display_help(char *program);

int main(int argc, char **argv){
    
    GRAPH g;
    NODE_DEVICE* nodes_host;
    EDGE_DEVICE* edges_host;
    TASK* tasks_host;
    CENSUS* census_host_NDRange;
    cl_uint num_nodes, num_edges, num_tasks, num_works;
    cl_ulong* census_host;

    cl_int err;  /*To control errs*/
    cl_platform_id platform;  /*Variables to set up environment*/
    cl_device_id device;
    cl_context context;
    cl_mem nodes_device, edges_device, tasks_device, census_device;
    cl_command_queue queue;  
    cl_program program;
    cl_kernel kernel; //Skernel_ordered;
    cl_event e;   /*Event arrays for synchronization*/
    struct timeval start, stop, start_1, stop_1;   /*For counting elapsed execution time*/
    //clock_t start, stop, start_1, stop_1;
    double reading_time, execution_time, reduction_time, conversion_time, setup_time;
    //double reading_time, execution_time, reduction_time;

    char input_name[MAX_CHAR], input_binary[MAX_CHAR], name_kernel[MAX_CHAR], times_name[MAX_CHAR];
    bool namefile, binary, function, times, times_file, result, verbose;
    int long_index;
    char opt;
    uint32_t i, j;

    static struct option options[] = {
        {"graph",required_argument,0,'1'},
        {"binary",required_argument,0,'2'},
        {"function",required_argument,0,'3'},
        {"times",optional_argument,0,'4'},
        {"result", no_argument,0,'5'},
        {"verbose", no_argument,0,'6'},
        {"help", no_argument,0,'7'},
        {0,0,0,0}
    };


    namefile = binary = function = times = times_file = result = verbose = false;
    long_index = 0;
    while ((opt = getopt_long_only(argc, argv,"1:2:3:4:567", options, &long_index)) != -1) {
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
                if (strlen(optarg) > MAX_CHAR){
                    printf("The input file name cannot excede %d characters\n", MAX_CHAR);
                    return EXIT_FAILURE;
                }
                strcpy(input_binary, optarg);
                binary = true;
                break;
            case '3' :   
                strcpy(name_kernel, optarg);
                function = true;  
                break;
            case '4' :
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
            case '5':
                result = true;
                break;
            case '6':
                verbose = true;
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

    


    gettimeofday(&start, NULL);

    g = read_graph_from_file(input_name);

    gettimeofday(&stop, NULL);

    reading_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;
    //reading_time = ((double) (stop - start)) / CLOCKS_PER_SEC;

    if (verbose || times)
        printf("Graph read in %lf seconds.\n", reading_time);

    
    if (g.empty()){
        printf("Error creating graph!!\n");
        return EXIT_FAILURE;
    }

    if (get_num_nodes(g) <=2){
        printf("Graph is too small to perform triadic census\n");
        g.clear();
        return EXIT_SUCCESS;
    }

    if (verbose || times){
    	display_graph_summary(g);
    }

    gettimeofday(&start, NULL);
    if (ERR == convert_graph(g, &nodes_host, &edges_host, &num_nodes, &num_edges)){
        printf("Error corverting graph!!");
        g.clear();
        free(nodes_host);
        free(edges_host);
        return EXIT_FAILURE;
    }
    gettimeofday(&stop, NULL);
    conversion_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;
    if (verbose || times)
    	printf("Graph converted in %lf seconds\n", conversion_time);

    if (NULL == (census_host = (cl_ulong*) calloc (NUM_TRIADS,sizeof(cl_ulong)))){
        printf("Error while allocating memory for census array\n");
        g.clear();
        free(nodes_host);
        free(edges_host);
        return EXIT_FAILURE;
    }

    gettimeofday(&start, NULL);
    //set up execution environment for kernels
    setup(&platform, &device, &context);
    if (verbose){
        display_platform_info(platform);
        display_device_info(device);
    }
    queue = clCreateCommandQueue(context, device, 0, &err);
    check_error(err, "Error creating command queue.");

    if (NULL == (program = create_build_program(input_binary, context, &device))){
        printf("An error ocurred while creating the program\n");
        return EXIT_FAILURE;
    }

    kernel = clCreateKernel(program, name_kernel, &err);
    check_error(err, "Error creating kernel.");
    /*CREATE DEVICE BUFFERS*/
    if (is_NDRange_kernel(name_kernel) && is_BM_kernel(name_kernel)){
        tasks_host = create_tasks_array(g, nodes_host, &num_tasks);
        tasks_device = create_and_write_buffer(context, queue, num_tasks*sizeof(TASK), tasks_host);
        num_works = num_tasks;
    } else {
    	nodes_device = create_and_write_buffer(context, queue, num_nodes*sizeof(NODE_DEVICE), nodes_host);
        num_works = num_nodes;
    }
    if (verbose || times)
        printf("Number of tasks launched is %d\n", num_works);

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
    gettimeofday(&stop_1, NULL);

    setup_time = (stop_1.tv_sec - start.tv_sec) + ((double) (stop_1.tv_usec - start.tv_usec))/1000000;

    if (verbose || times)
    	printf("Setup performed in %lf seconds\n", setup_time);
    if (verbose)
        printf("Performing triadic census on kernel %s...\n", name_kernel);


    if (is_NDRange_kernel(name_kernel)){
        const size_t globalWorkSize = num_works;
        err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, &globalWorkSize, NULL, 0, NULL, &e);
        check_error(err, "error executing NDRange kernel.");
        err = clEnqueueReadBuffer(queue, census_device, CL_TRUE, 0, num_works*sizeof(CENSUS), census_host_NDRange, 1, &e, NULL);
        check_error(err, "error reading buffer.");
        gettimeofday(&start_1, NULL);
        for(i=0;i<num_works;i++){
            for (j=0;j<NUM_TRIADS;j++){
                census_host[j]+=census_host_NDRange[i].census[j];
            }
        }
        gettimeofday(&stop_1, NULL);
        reduction_time = (stop_1.tv_sec - start_1.tv_sec) + ((double) (stop_1.tv_usec - start_1.tv_usec))/1000000;

        if (verbose || times)
            printf("Reduction performed in %lf seconds\n", reduction_time);
    } else {
        printf("ENTRO AQUI!!!!\n");
        printf("kernel = %d\n", kernel);
        printf("queue = %d\n", queue);
        err = clEnqueueTask(queue, kernel, 0, NULL, &e);
        check_error(err, "error executing task.");
        err = clEnqueueReadBuffer(queue, census_device, CL_TRUE, 0, NUM_TRIADS*sizeof(cl_ulong), census_host, 1, &e, NULL);
    } 
    gettimeofday(&stop, NULL);
    //stop = clock();
    execution_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_usec - start.tv_usec))/1000000;
    //execution_time = ((double) (stop - start)) / CLOCKS_PER_SEC;
    if (is_BM_kernel(name_kernel)){
        uint64_t sum = 0;
        for(j=1;j<NUM_TRIADS;j++){
            sum+= census_host[j];
        }
        census_host[0] = num_total_triads(get_num_nodes(g)) - sum;
    }
    if (verbose || times)
        printf("Triadic census performed in %lf seconds.\n", execution_time);

    if (verbose || result){
        printf("Triadic census:\n");
        print_triad_census(census_host);
    }
    if (times_file){
    	write_times(times_name, g, reading_time, execution_time);
    }
    

    
    clReleaseMemObject(nodes_device); clReleaseMemObject(edges_device); clReleaseMemObject(census_device);
    
    clReleaseEvent(e);


    free(nodes_host);
    free(edges_host);
    free(census_host);

    g.clear();

    clReleaseContext(context);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    if (verbose)
        printf("Execution finished successfully\n");

    return EXIT_SUCCESS;
}



void display_help(char *program){
    printf("\nProgram to perform the triadic census of a graph.\n");
    printf("\nUse this program if you want to test correctness or performance of a triadic census algorithm on a given graph\n\n");
    printf("Usage: %s -g <namefile> -b <aocx file> -f <kernel_name> [-t[=<timefile>]] [-r] [-v] [-o[=deg|deg-r]] [-h]\n", program);
    printf("Where:\n");
    printf("\t-g, --graph <namefile>: file containing input graph\n");
    printf("\t-b, --binary <aocx file>: aocx file containing bitstream for kernel functions\n");
    printf("\t-f, --function <kernel_name>: name of the kernel synthetized on the aocx file\n");
    printf("\t-t, --times [<timefile>]: Option to display times of execution\n");
    printf("\t-r, --result: Option to display results of triadic census\n");
    printf("\t-v, --verbose: Option to display info about execution process\n");
    printf("\t-o, --ordered[=deg|deg-r]: Option to save nodes and adjacencies in order (default is not-ordered)\n");
    printf("\t-h, --help: Option to display help\n");
}