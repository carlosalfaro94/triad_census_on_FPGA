
#include "../headers/aux_opencl.h"


/**
  *   @file aux_opencl.c
  *
  *   @brief This file contains the code that implements the functions defined in the 
  *   header file aux_opencl.h. Please refer to it to check the documentation.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 18-12-2017
  **/



void setup(cl_platform_id* platform, cl_device_id* device, cl_context* context){
    cl_int err;

    //1. Query and select the platforms
    err = clGetPlatformIDs(1, platform, NULL);
    check_error(err, "Error getting platform IDs.");

    //2. Get the first FPGA device in the platform
    err = clGetDeviceIDs(*platform, CL_DEVICE_TYPE_ACCELERATOR, 1, device, NULL);
    check_error(err, "Error getting device IDs.");

    //3. Create an OpenCL context for the FPGA device
    *context = clCreateContext(NULL, 1, device, NULL, NULL, &err);
    check_error(err, "Error creating context.");

}



cl_program create_build_program(char *filename, cl_context context, cl_device_id* device){
    //create program from precompiled binary
    cl_int err;
    cl_program program;
    
    FILE *fp;   /*Need to extract the char stream and the length in bytes of the file*/
    size_t nbytes;
    unsigned char *binary;


    if (NULL == (fp = fopen(filename, "r"))){
        printf("Error while opening the file %s\n", filename);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    nbytes = ftell(fp);
    
    if (NULL == (binary = (unsigned char*) malloc(sizeof(unsigned char)*nbytes))){
        printf("Error while allocating memory for binary bit stream\n");
        fclose(fp);
        return NULL;
    }
    rewind(fp);
    size_t temp;   //Maybe here an error check would not be bad
    if (0 == (temp = fread((void *) binary, nbytes, 1, fp))){
        printf("Problem reading binary aocx file\n");
        free(binary);
        fclose(fp);
        return NULL;
    }
    
    program = clCreateProgramWithBinary(context, 1, device, (const size_t *) &nbytes, (const unsigned char **) &binary, NULL, &err);
    check_error(err, "Error extracting program from binary");

    err = clBuildProgram(program, 1, device, NULL, NULL, NULL);
    if (err != CL_SUCCESS){
        printf("Non CL_SUCCESS when building program.\n");
        return NULL;
    }
    free(binary);
    fclose(fp);
    return program;
}

void display_platform_info(cl_platform_id platform){
    char char_buffer[STRING_BUFFER_LEN]; 
    printf("Querying platform for info:\n");
    printf("==========================\n");
    clGetPlatformInfo(platform, CL_PLATFORM_NAME, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n", "CL_PLATFORM_NAME", char_buffer);
    clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n", "CL_PLATFORM_VENDOR ", char_buffer);
    clGetPlatformInfo(platform, CL_PLATFORM_VERSION, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n\n", "CL_PLATFORM_VERSION ", char_buffer);
}

void display_device_info(cl_device_id device){
    char char_buffer[STRING_BUFFER_LEN];
    cl_uint int_buffer;
    size_t array_buffer[3];
    size_t size_buffer;
    cl_ulong mem_size;
    printf("Querying device for info:\n");
    printf("========================\n");
    clGetDeviceInfo(device, CL_DEVICE_NAME, STRING_BUFFER_LEN, &char_buffer, NULL);
    printf("%-40s = %s\n", "CL_DEVICE_NAME", char_buffer);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, STRING_BUFFER_LEN, &char_buffer, NULL);
    printf("%-40s = %s\n", "CL_DEVICE_VENDOR", char_buffer);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, STRING_BUFFER_LEN, &char_buffer, NULL);
    printf("%-40s = %s\n", "CL_DEVICE_VENDOR_ID", char_buffer);
    clGetDeviceInfo(device, CL_DEVICE_VERSION, STRING_BUFFER_LEN, &char_buffer, NULL);
    printf("%-40s = %s\n", "CL_DEVICE_VERSION", char_buffer);
    clGetDeviceInfo(device, CL_DRIVER_VERSION, STRING_BUFFER_LEN, &char_buffer, NULL);
    printf("%-40s = %s\n", "CL_DRIVER_VERSION", char_buffer);
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &int_buffer, NULL);
    printf("%-40s = %u\n", "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS", int_buffer);
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 3*sizeof(size_t), &array_buffer, NULL);
    printf("%-40s = [%zd, %zd, %zd]\n", "CL_DEVICE_MAX_WORK_ITEM_SIZES", array_buffer[0], array_buffer[1], array_buffer[2]);
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &size_buffer, NULL);
    printf("%-40s = %zd\n", "CL_DEVICE_MAX_WORK_ITEM_SIZES", size_buffer);
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
    printf("%-40s = %zd\n", "CL_DEVICE_LOCAL_MEM_SIZE", mem_size);
}



void check_error(cl_int error_code, const char* message){
    if (CL_SUCCESS == error_code){
        return;
    }
    printf(message);
    switch(error_code){
        case CL_INVALID_PLATFORM:
            printf("CL_INVALID_PLATFORM\n");
            break;
        case CL_INVALID_DEVICE_TYPE:
            printf("CL_INVALID_DEVICE_TYPE\n");
            break;
        case CL_INVALID_DEVICE:
            printf("CL_INVALID_DEVICE\n");
            break;
        case CL_INVALID_CONTEXT:
            printf("CL_INVALID_CONTEXT\n");
            break;
        case CL_OUT_OF_HOST_MEMORY:
            printf("CL_OUT_OF_HOST_MEMORY\n");
            break;
        case CL_DEVICE_NOT_FOUND:
            printf("CL_DEVICE_NOT_FOUND\n");
            break;
        case CL_INVALID_KERNEL_NAME:
            printf("CL_INVALID_KERNEL_NAME\n");
            break;
        case CL_INVALID_BINARY:
            printf("CL_INVALID_BINARY\n");
            break;
        case CL_INVALID_VALUE:
            printf("CL_INVALID_VALUE\n");
            break;
        default:
            printf("Unknown error code (%d).\n", error_code);
            break;

    }
    exit(EXIT_FAILURE);
}


cl_mem create_and_write_buffer(cl_context context, cl_command_queue queue, size_t size, void* host_ptr){
	cl_mem dev_ptr;
	cl_int err;
	dev_ptr = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, &err);
	check_error(err, "Error creating device buffer.");
	err = clEnqueueWriteBuffer(queue, dev_ptr, CL_TRUE, 0, size, host_ptr, 0, NULL, NULL);
	check_error(err, "Error writing device buffer.");
	return dev_ptr;	
}

void set_args(cl_kernel kernel, void* arg0, void* arg1, void* arg2, void* arg3){
	cl_int err;
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), arg0);
    check_error(err, "Error setting kernel arg 0.");

    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), arg1);
    check_error(err, "Error setting kernel arg 1.");


    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), arg2);
    check_error(err, "Error setting kernel arg 2.");

    err = clSetKernelArg(kernel, 3, sizeof(cl_uint), arg3);
    check_error(err, "Error setting kernel arg 3");

}

