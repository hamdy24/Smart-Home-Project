/*
 * GIE_prog.c
 *
 *  Created on: Oct 30, 2021
 *      Author: hamdy
 */

#include "../../LIBRARY/stdTypes.h"
#include "../../LIBRARY/errorStates.h"

#include "GIE_priv.h"

ES_t GIE_enuInit(void)
{
	ES_t Local_ErorrState = ES_NOK ;

	SREG &= ~(1<<7);		// GIE BIT FROM SREG REGEISTER

	Local_ErorrState = ES_OK;

	return Local_ErorrState;
}
ES_t GIE_enuEnable(void)
{
	ES_t Local_ErorrState = ES_NOK ;

	SREG |= (1<<7);

	Local_ErorrState = ES_OK;
	return Local_ErorrState;
}

ES_t GIE_enuDisable(void)
{
	ES_t Local_ErorrState = ES_NOK ;

	SREG &= ~(1<<7);

	Local_ErorrState = ES_OK;
	return Local_ErorrState;
}

