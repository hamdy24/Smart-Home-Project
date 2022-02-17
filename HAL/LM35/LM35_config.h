/*
 * LM35_config.h
 *
 *  Created on: 23 Oct 2021
 *      Author: Dell
 */

#ifndef HAL_LM35_CONFIG_H_
#define HAL_LM35_CONFIG_H_

/****************#define 	LM35_u8_PORT		DIO_u8GROUP_A    wrong******************/
/***************It must be Connected At Port A for ADC to be used******************/
#define		LM35_u8PIN								DIO_u8PIN1

#define		LM35_PRESET_FOR_ADC_TRIGGERING			ADC_u8Free_Running_Mode




#endif /* HAL_LM35_CONFIG_H_ */
