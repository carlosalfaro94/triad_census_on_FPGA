
#include "CL/opencl.h"
#include "aux.h"

#define MAX_CHAR 256
#define STRING_BUFFER_LEN 1024


void setup(cl_platform_id* platform, cl_device_id* device, cl_context* context);

cl_program create_build_program(char *filename, cl_context context, cl_device_id* device);

void display_platform_info(cl_platform_id platform);

void display_device_info(cl_device_id device);

void check_error(cl_int error_code, const char* message);

cl_mem create_and_write_buffer(cl_context context, cl_command_queue queue, size_t size, void* host_ptr);

void set_args(cl_kernel kernel, void* arg0, void* arg1, void* arg2, void* arg3);


