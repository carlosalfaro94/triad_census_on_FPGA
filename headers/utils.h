/**
  *   @file utils.h
  *
  *   @brief This file contains the enumerations and macros needed for the whole project
  *
  *   @author: Carlos Alfaro
  *
  *   @date: 13-10-2017
  **/


#define MAX_NEIGHBORS 100000    /*!< Maximum number of neighbors supported */
#define NUM_TRIADS 16			/*!< Number of isomorphic triad classes */

typedef enum {
	FALSE = 0, 			/*!< Boolean value FALSE */
	TRUE				/*!< Boolean value FALSE */
} BOOL;   /*! Boolean enumeration */

typedef enum {
	ERR=-1,    			/*!< Error */
	OK					/*!< Successful execution */
} STATUS; /*! Status enumeration (for return values) */

typedef enum {
	NONE = 0,  			/*!< No edge */ 
	IN_OUT, 			/*!< Outgoing edge */
	OUT_IN, 			/*!< Incoming edge */
	BIDIRECTIONAL		/*!< Bidirectional edge */
} DIRECTION;  /*! Edge direction enumeration */

