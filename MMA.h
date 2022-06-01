
/** \file MMA.h
 * Modified Moving Average Header File
 * 
 */

/*! \mainpage Modified Moving Average
 *
 * \section Description
 *
 * This library provides up to [MAX_AVERAGES] unique modified moving averages. Each average is first registered and then accessed by using an avgID with the provided functions.
 *
 * \note This library does not use dynamic memory as it is intended for embedded projects.  All memory is assigned at compile time.  Use [MAX_AVERAGES] to set the amount of memory used.
 *
 * \section How To Use
 *
 * Update [NUMBER_OF_SAMPLES] to the number of values to be summed for the average.
 *
 * Update [MAX_AVERAGES] to the number of MMA averages that are required.
 *
 * Register with the module by calling avg_register(uint8_t *avgID).  check the return value of the function before accessing the parameter. On function return, the parameter *avgID holds the ID that can be used with the rest of the functions.
 *
 * To add a new value to the average, call avg_moveTheAverage(int8_t avgID, uint16_t newValue), where newValue is the newest number to add to the average.  
 *
 * To get the current average, call mma_error_t avg_getAverage(int8_t avgID, uint16_t *average)
 *
 * When finished with an average, call mma_error_t avg_free(int8_t avgID) to free the ID so it can be used elsewhere
 *
 * To reset the average to 0, call mma_error_t avg_reset(int8_t avgID)
 *
 */

//Number of values to use for the moving average
#define NUMBER_OF_SAMPLES   8
//Number of averages needed by external modules
#define MAX_AVERAGES 2 

typedef enum AverageState{
	AVG_UNUSED,
	AVG_USED
}avgState_t;

typedef enum MMA_Error{
	MMA_ERR_OUT_OF_BOUND = -4,
	MMA_ERR_ALL_ID_USED,
	MMA_ERR_ID_UNREGISTERED,
	MMA_ERR_OVERFLOW,
	MMA_SUCCESS	
}mma_error_t;

mma_error_t avg_register(uint8_t *avgID);
mma_error_t avg_free(uint8_t avgID);
mma_error_t avg_moveTheAverage(uint8_t avgID, uint16_t newValue);
mma_error_t avg_getAverage(uint8_t avgID, uint16_t *average);
mma_error_t avg_reset(uint8_t avgID);

