/*
 * ADC_int.h
 *
 *  Created on: 25 Oct 2021
 *      Author: Dell
 */

#ifndef MCAL_ADC_INT_H_
#define MCAL_ADC_INT_H_

ES_t ADC_enuInit (void);

ES_t ADC_enuStartConversion (void);

ES_t ADC_enuChooseChannel(u8 Copy_u8ChannelID);

ES_t ADC_enuEnableAutoTriggering (u8 Copy_u8TriggeringMode);

ES_t ADC_enuDisableAutoTriggering (void);

ES_t ADC_enuPollying(void);

ES_t ADC_enuEnableADC_Inturrupt(void);

ES_t ADC_enuDisableADC_Inturrupt(void);

ES_t ADC_enuEnableADC(void);

ES_t ADC_eniDisableADC(void);

ES_t ADC_enuReadDataReg (u16 * Copy_pu16ReadValue);

ES_t ADC_enuReadHigherDataReg (u8 * Copy_pu8HigherReg);

ES_t ADC_enuCallBack (volatile void (* Copy_pfun)(void));


#define	  ADC_u8SINGLE_CH_ADC0		0
#define	  ADC_u8SINGLE_CH_ADC1		1
#define	  ADC_u8SINGLE_CH_ADC2		2
#define	  ADC_u8SINGLE_CH_ADC3		3
#define	  ADC_u8SINGLE_CH_ADC4		4
#define	  ADC_u8SINGLE_CH_ADC5		5
#define	  ADC_u8SINGLE_CH_ADC6		6
#define	  ADC_u8SINGLE_CH_ADC7		7

#define	  ADC_u8DIFF_CH_ADC0_ADC0_10x		8
#define	  ADC_u8DIFF_CH_ADC1_ADC0_10x		9
#define	  ADC_u8DIFF_CH_ADC0_ADC0_200x		10
#define	  ADC_u8DIFF_CH_ADC1_ADC0_200x		11
#define	  ADC_u8DIFF_CH_ADC2_ADC2_10x		12
#define	  ADC_u8DIFF_CH_ADC3_ADC2_10x		13
#define	  ADC_u8DIFF_CH_ADC2_ADC2_200x		14
#define	  ADC_u8DIFF_CH_ADC3_ADC2_200x		15
#define	  ADC_u8DIFF_CH_ADC0_ADC1_1x		16
#define	  ADC_u8DIFF_CH_ADC1_ADC1_1x		17
#define	  ADC_u8DIFF_CH_ADC2_ADC1_1x		18
#define	  ADC_u8DIFF_CH_ADC3_ADC1_1x		19
#define	  ADC_u8DIFF_CH_ADC4_ADC1_1x		20
#define	  ADC_u8DIFF_CH_ADC5_ADC1_1x		21
#define	  ADC_u8DIFF_CH_ADC6_ADC1_1x		22
#define	  ADC_u8DIFF_CH_ADC7_ADC1_1x		23
#define	  ADC_u8DIFF_CH_ADC0_ADC2_1x		24
#define	  ADC_u8DIFF_CH_ADC1_ADC2_1x		25
#define	  ADC_u8DIFF_CH_ADC2_ADC2_1x		26
#define	  ADC_u8DIFF_CH_ADC3_ADC2_1x		27
#define	  ADC_u8DIFF_CH_ADC4_ADC2_1x		28




#define		ADC_s8SINGLE_CONVERSION 	   	 -1

#define		ADC_u8Free_Running_Mode	    		0
#define		ADC_u8Analog_Comparator				1
#define		ADC_u8External_Interrupt_Request_0	2
#define		ADC_u8Timer0_Compare_Match			3
#define		ADC_u8Timer0_Overflow				4
#define		ADC_u8Timer_Compare_Match_B			5
#define		ADC_u8Timer1_Overflow				6
#define		ADC_u8Timer1_Capture_Event			7




#endif /* MCAL_ADC_INT_H_ */
