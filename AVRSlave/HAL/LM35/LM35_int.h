/*
 * LM35_int.h
 *
 *  Created on: 23 Oct 2021
 *      Author: Dell
 */

#ifndef HAL_LM35_INT_H_
#define HAL_LM35_INT_H_

#define		ADC_u8Flag_Not_Raised		0

ES_t LM35_enuInit (void);

ES_t LM35_enuGetTemp(u16 * Copy_u16TempVal);

ES_t LM35_enuGetTempByPollying(u16 * Copy_u16TempVal);

void LM35_RaiseFlag(void);

#endif /* HAL_LM35_INT_H_ */
