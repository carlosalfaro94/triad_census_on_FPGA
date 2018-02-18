#include "CL/opencl.h"
#include "aux.h"


/**
  *   @file aux_opencl.h
  *
  *   @brief This file contains the definitions of the auxiliary functions\n
  *	  		 used by the main program run the OpenCL application. It includes functions\n
  *			 to create the objects necessary to execute the application.
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 18-12-2017
  **/

#define MAX_CHAR 256
#define STRING_BUFFER_LEN 1024

/**
  * @brief Function sets up the proper OpenCL environment for creating and executing kernels.\n
  * This process involves selecting an OpenCL platform and an attached device, as well as creating an execution context.  
  * 
  * @param cl_platform_id* platform: Pointer to the OpenCL platform (to be filled by the function)
  *	@param cl_device_id* device: Pointer to the OpenCL device id (to be filled by the function)
  * @param cl_context* context: Pointer to the OpenCL context (to be created by the function)
  * 
  *	@return None
  *
  **/
void setup(cl_platform_id* platform, cl_device_id* device, cl_context* context);

/**
  * @brief Function creates and builds the program from a binary .aocx file\n
  * This process involves selecting an OpenCL platform and an attached device, as well as creating an execution context.  
  * 
  * @param char *filename: Name of the .aocx file containing the binary HDL file	
  * @param cl_context context: OpenCL context
  * @param cl_device_id* device: Pointer to the OpenCL device id
  * 
  *	@return None
  *
  **/
cl_program create_build_program(char *filename, cl_context context, cl_device_id* device);

/**
  * @brief Function displays information about the OpenCL platform  
  * 
  * @param cl_platform_id platform: OpenCL platform 	
  *
  *	@return None
  *
  **/
void display_platform_info(cl_platform_id platform);

/**
  * @brief Function displays information about the OpenCL device used 
  * 
  * @param cl_device_id device: OpenCL device 	
  *
  *	@return None
  *
  **/
void display_device_info(cl_device_id device);

/**
  * @brief Function checks the error code returned by a API function. If there has been\n
  * an error, displays the errror code and stops execution.
  * 
  * @param cl_int error_code: error code returned by the function
  * @param const char* message: Message to be printed
  *
  *	@return None
  *
  **/
void check_error(cl_int error_code, const char* message);

/**
  * @brief Function that creates an OpenCL buffer object and fills it with certain information
  * 
  * @param cl_context context: OpenCL context
  * @param cl_command_queue queue: OpenCL queue for the communication between host and OpenCL device
  * @param size_t size: Size in bytes of the buffer
  * @param void* host_ptr: Pointer to the data to be copied in the newly created buffer.
  *
  *	@return cl_mem The buffer created.
  *
  **/
cl_mem create_and_write_buffer(cl_context context, cl_command_queue queue, size_t size, void* host_ptr);

/**
  * @brief Function that sets the four argument of an OpenCL kernel
  * 
  * @param cl_kernel kernel: The kernel we want to set the params to
  * @param void* arg0: First argument
  * @param void* arg1: Seconde argument 
  * @param void* arg2: Third argument 
  * @param void* arg3: Fourth argument 
  *
  *	@return None
  *
  **/
void set_args(cl_kernel kernel, void* arg0, void* arg1, void* arg2, void* arg3);


