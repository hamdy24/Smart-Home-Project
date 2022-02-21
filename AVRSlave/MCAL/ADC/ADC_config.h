/*
 * ADC_config.h
 *
 *  Created on: 25 Oct 2021
 *      Author: Dell
 */

#ifndef MCAL_ADC_CONFIG_H_
#define MCAL_ADC_CONFIG_H_

#define 	ADC_u8REFERENCE_VOLTAGE			ADC_u8AVCC_PIN
/*********Reference Options: 1- ADC_u8AREF_PIN	2-ADC_u8AVCC_PIN	3-ADC_u8INTERNAL_REF***********/


#define 	ADC_u8DATA_REG_ADJUSTMENT		ADC_u8LEFT_ADJUSMENT
/*********ADC Data Reg Adjustment Options: 1- ADC_u8RIGHT_ADJUSMENT	2-ADC_u8LEFT_ADJUSMENT***********/


#define 	ADC_u8PRESCALAR					ADC_u8PRESC64
/*********ADC Prescalar Options: 1- ADC_u8PRESC2	2-ADC_u8PRESC4	 3-ADC_u8PRESC8		4-ADC_u8PRESC16		5-ADC_u8PRESC32		6-ADC_u8PRESC64		7-ADC_u8PRESC128***********/


#define		ADC_u8CHANNEL					ADC_u8SINGLE_CH_ADC1
/*********ADC Channel Options: In ADC_int.h***********/


#define		ADC_u8STATE						ADC_u8DISABLED
/*********ADC State Options: 1-ADC_u8DISABLED	2-ADC_u8ENABLED***********/


#define		ADC_u8TRIGGERING_MODE			ADC_u8External_Interrupt_Request_0
/*********ADC Triggering Options : In ADC_int.h***********/


#define		ADC_u8INTERRUPT_STATE			ADC_u8INTERRUPT_OFF
/*********ADC Interrupt State Options : I-ADC_u8INTERRUPT_OFF	2-ADC_u8INTERRUPT_ON***********/








#endif /* MCAL_ADC_CONFIG_H_ */
