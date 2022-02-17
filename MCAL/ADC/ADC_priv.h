/*
 * ADC_priv.h
 *
 *  Created on: 25 Oct 2021
 *      Author: Dell
 */

#ifndef MCAL_ADC_PRIV_H_
#define MCAL_ADC_PRIV_H_

#define 	ADC_u8AREF_PIN			0
#define 	ADC_u8AVCC_PIN			1
#define 	ADC_u8INTERNAL_REF		3		//ob11 in binary

#define 	ADC_u8RIGHT_ADJUSMENT	0
#define		ADC_u8LEFT_ADJUSMENT	1

enum ADCPrescalar
{
	ADC_u8PRESC2 = 1,
	ADC_u8PRESC4,
	ADC_u8PRESC8,
	ADC_u8PRESC16,
	ADC_u8PRESC32,
	ADC_u8PRESC64,
	ADC_u8PRESC128
};

#define		ADC_u8DISABLED		0
#define		ADC_u8ENABLED		1

#define		ADC_u8INTERRUPT_OFF		0
#define		ADC_u8INTERRUPT_ON		1

enum ADC_Channel
{
	ADC_u8SINGLE_CH_ADC0,
	ADC_u8SINGLE_CH_ADC1,
	ADC_u8SINGLE_CH_ADC2,
	ADC_u8SINGLE_CH_ADC3,
	ADC_u8SINGLE_CH_ADC4,
	ADC_u8SINGLE_CH_ADC5,
	ADC_u8SINGLE_CH_ADC6,
	ADC_u8SINGLE_CH_ADC7,

	ADC_u8DIFF_CH_ADC0_ADC0_10x,
	ADC_u8DIFF_CH_ADC1_ADC0_10x,
	ADC_u8DIFF_CH_ADC0_ADC0_200x,
	ADC_u8DIFF_CH_ADC1_ADC0_200x,
	ADC_u8DIFF_CH_ADC2_ADC2_10x,
	ADC_u8DIFF_CH_ADC3_ADC2_10x,
	ADC_u8DIFF_CH_ADC2_ADC2_200x,
	ADC_u8DIFF_CH_ADC3_ADC2_200x,
	ADC_u8DIFF_CH_ADC0_ADC1_1x,
	ADC_u8DIFF_CH_ADC1_ADC1_1x,
	ADC_u8DIFF_CH_ADC2_ADC1_1x,
	ADC_u8DIFF_CH_ADC3_ADC1_1x,
	ADC_u8DIFF_CH_ADC4_ADC1_1x,
	ADC_u8DIFF_CH_ADC5_ADC1_1x,
	ADC_u8DIFF_CH_ADC6_ADC1_1x,
	ADC_u8DIFF_CH_ADC7_ADC1_1x,
	ADC_u8DIFF_CH_ADC0_ADC2_1x,
	ADC_u8DIFF_CH_ADC1_ADC2_1x,
	ADC_u8DIFF_CH_ADC2_ADC2_1x,
	ADC_u8DIFF_CH_ADC3_ADC2_1x,
	ADC_u8DIFF_CH_ADC4_ADC2_1x
};

enum ADC_Triggering_Modes
{
	ADC_s8SINGLE_CONVERSION = -1,

	ADC_u8Free_Running_Mode,
	ADC_u8Analog_Comparator,
	ADC_u8External_Interrupt_Request_0,
	ADC_u8Timer0_Compare_Match,
	ADC_u8Timer0_Overflow,
	ADC_u8Timer_Compare_Match_B,
	ADC_u8Timer1_Overflow,
	ADC_u8Timer1_Capture_Event

};


/*********************Registers Definitions*********************/
#define		ADMUX		*((volatile u8*)0x27)
#define		ADCSRA		*((volatile u8*)0x26)
#define		ADCH		*((volatile u8*)0x25)
#define		ADCL		*((volatile u8*)0x24)
#define		SFIOR		*((volatile u8*)0x50)
/****************************************************************/


/*********************Registers Bits Definitions*********************/

/*********************ADMUX Bits Definitions*********************/
#define		REF_S0		6
#define 	ADLAR		5
#define 	MUX0		0

/*********************ADCSRA Bits Definitions*********************/
#define 	ADPS0		0
#define		ADATE		5
#define 	ADIE		3
#define		ADEN		7
#define		ADSC		6
#define		ADIF		4

/*********************SFIOR Bits Definitions*********************/
#define 	ADTS0		5

/********************************************************************/


#define		ADC_u8Flag_Not_Raised		0

#endif /* MCAL_ADC_PRIV_H_ */
