#include "utils.h"

/**
  *   @file defs.h
  *
  *   @brief This file contains the definitions of the structures that are used for the kernel code
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 13-12-2017
  **/

/**
 *  @struct EDGE_DEVICE
 *  @brief This structure stores the information of a certain edge of the graph.
 *  This structure is used in the kernel code and executed on the FPGA 
 */
typedef unsigned int EDGE_DEVICE;   /*!< Stores the neighbor id and the direction of the edge */


/**
 *  @struct NODE_DEVICE
 *  @brief This structure stores the information of a node within the graph.
 *  This structure is used in the kernel code and executed on the FPGA 
 */
typedef struct {
	unsigned int node_id; 		/*!< The id of the node */
	unsigned int first_ind;   	/*!< The index of the beginning of the edge subarray*/
	unsigned int last_ind;		/*!< The index of the final of the edge subarray*/
} NODE_DEVICE;

/**
 *  @struct TASK
 *  @brief This structure contains the information of a pair of nodes (u,v).
 *  Each pair of nodes is processed by a different kernel instance on a SIMD manner
 *  This structure is used in the kernel code and executed on the FPGA 
 */
typedef struct {
	NODE_DEVICE u;				/*!< The first node of the pair that conforms the TASK */
	NODE_DEVICE v;				/*!< The second node of the pair that conforms the TASK */
}TASK;

/**
 *  @struct CENSUS
 *  @brief This structure contains a 16-position census array
 *  Each instance of an NDRange kernel has a different census array in which to write local results.
 *  This structure is used in the kernel code and executed on the FPGA 
 */
typedef struct {
	unsigned long census[NUM_TRIADS];     /*!< 16-position census array*/
}CENSUS;
