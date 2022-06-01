/** \file MMA.c
 * Modified Moving Average c file
 * 
 */

#include <avr/io.h>
#include <stdio.h>
#include "MMA.h"
#include "UART.h"

/**
* Struct definition for moving average
*/

struct MovingAverage{
	avgState_t state; /**< USED or UNUSED */
	uint16_t average; /**< Holds the current moving average */
	uint32_t sum; /**< Holds the sum of the previous [NUMBER_OF_SAMPLES] samples */	
};

/**
* Struct array to hold all the averages
*/

static struct MovingAverage mmaList [MAX_AVERAGES];

/**
*	\brief Register for an ID
*
*	\details This function returns an ID that can used to calculate an
*	average for a particular instance
*
*	\return Returns an ID > 0, or negative value on error
*/

mma_error_t avg_register(uint8_t *avgID){
	//find the next unused ID and return
	for (uint8_t i=0; i < MAX_AVERAGES; i++){
		struct MovingAverage *mma = &mmaList[i];
		if (mma->state == AVG_UNUSED){
			//unused average found, initialize and return avgID to be used
			mma->average = 0;
			mma->state = AVG_USED;
			mma->sum = 0;			
			*avgID = i;
			return MMA_SUCCESS;
		}
	}
	return MMA_ERR_ALL_ID_USED;
}

/**
* \brief Free an ID
*
* \details Sets an ID to be freed so that it can be used elsewhere
*
* \param[in] avgID	The ID to be freed
*
* \return Returns 0 if successfully freed, returns negative on error
*/

mma_error_t avg_free(uint8_t avgID){
	if (avgID >= 0 && avgID < MAX_AVERAGES){
		struct MovingAverage *mma = &mmaList[avgID];
		if (mma->state != AVG_UNUSED){
			return MMA_ERR_ID_UNREGISTERED;
		}
		mma->average = 0;
		mma->state = AVG_UNUSED;
		mma->sum = 0;		
	}
	else{
		return MMA_ERR_OUT_OF_BOUND;
	}
	return MMA_SUCCESS;
}

/**
* \brief Move the Average
*
* \details This function updates the average with the newest value.  It also detects overflows and returns an error
* \param[in]	avgID		The ID to update
* \param[in]	newValue	The newest value to add to the moving average
*
* \return	Returns 0 on success and a negative value on error
*/

mma_error_t avg_moveTheAverage(uint8_t avgID, uint16_t newValue){
	if (avgID >= 0 && avgID < MAX_AVERAGES){
		struct MovingAverage *mma = &mmaList[avgID];				
		
		//detect overflow
		if ( (65535 - mma->sum + mma->average) < (newValue) ){//(mma->sum - mma->average + newValue) < mma->sum){
			return MMA_ERR_OVERFLOW;
		}
		
		//move the average
		mma->sum -= mma->average;
		mma->sum += newValue;
		mma->average = mma->sum / NUMBER_OF_SAMPLES;
	}
	return MMA_ERR_OUT_OF_BOUND;
}

/**
* \brief Get average
*
* \details get the average from the ID supplied
*
* \param[in]	avgID		ID to use
* \param[out]	*average	updates variable with current moving average
*
* \return	returns 0 on success, negative value on error
*/

mma_error_t avg_getAverage(uint8_t avgID, uint16_t *average){
	if (avgID >= 0 && avgID < MAX_AVERAGES){
		struct MovingAverage *mma = &mmaList[avgID];
		if (mma->state != AVG_USED){
			return MMA_ERR_ID_UNREGISTERED;
		}
		*average = mma->average;
		return MMA_SUCCESS;
	}
	return MMA_ERR_OUT_OF_BOUND;
}

/**
* \brief Resets Average
*
* \details Resets the average and counter for supplied ID
*
* \param[in]	avgID	ID to use
*
* \return	returns 0 on success, negative value on error
*/

mma_error_t avg_reset(uint8_t avgID){
	if (avgID >= 0 && avgID < MAX_AVERAGES){
		struct MovingAverage *mma = &mmaList[avgID];
		if (mma->state != AVG_USED){
			return MMA_ERR_ID_UNREGISTERED;
		}
		mma->average = 0;		
		mma->state = AVG_USED; //we still want to keep it
		mma->sum = 0;
		return MMA_SUCCESS;
	}
	return MMA_ERR_OUT_OF_BOUND;
}
