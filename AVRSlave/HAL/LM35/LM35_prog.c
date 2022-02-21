/*
 * LM35_prog.c
 *
 *  Created on: 23 Oct 2021
 *      Author: Dell
 */
#include "../../LIBRARIES/stdTypes.h"
#include "../../LIBRARIES/errorStates.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "../../MCAL/ADC/ADC_int.h"

#include "LM35_priv.h"
#include "LM35_config.h"

static u8 LM35_u8Flag = 0;

void LM35_RaiseFlag (void)
{
	LM35_u8Flag = 1;
}

ES_t LM35_enuInit (void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	u16 Local_u16ErrorStatus;
	if (LM35_u8PIN <= DIO_u8PIN7)
	{
		Local_u16ErrorStatus = DIO_enuSetPinDirection(DIO_u8GROUP_A ,LM35_u8PIN, DIO_u8INPUT);
		DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN1, DIO_u8FLOAT);
		Local_u16ErrorStatus |= (ADC_enuInit()<<2);
/*
#if LM35_u8PIN == DIO_u8PIN0
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC0)<<4);
#elif LM35_u8PIN == DIO_u8PIN1
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC1)<<4);
#elif LM35_u8PIN == DIO_u8PIN2
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC2)<<4);
#elif LM35_u8PIN == DIO_u8PIN3
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC3)<<4);
#elif LM35_u8PIN == DIO_u8PIN4
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC4)<<4);
#elif LM35_u8PIN == DIO_u8PIN5
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC5)<<4);
#elif LM35_u8PIN == DIO_u8PIN6
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC6)<<4);
#elif LM35_u8PIN == DIO_u8PIN7
		Local_u16ErrorStatus |= (ADC_enuChooseChannel(ADC_u8SINGLE_CH_ADC7)<<4);
#endif

*/


		Local_u16ErrorStatus |= (ADC_enuCallBack((volatile void (*))LM35_RaiseFlag)<<6);
		Local_u16ErrorStatus |= (ADC_enuEnableADC_Inturrupt()<<8);

#if LM35_PRESET_FOR_ADC_TRIGGERING == ADC_u8External_Interrupt_Request_0
		Local_u16ErrorStatus |= (DIO_enuSetPinDirection(DIO_u8GROUP_D ,DIO_u8PIN2, DIO_u8INPUT)<<12);
		DIO_enuSetPinValue(DIO_u8GROUP_D, DIO_u8PIN2, DIO_u8FLOAT);
		Local_u16ErrorStatus |= (ADC_enuEnableAutoTriggering(LM35_PRESET_FOR_ADC_TRIGGERING)<<14);
#elif (LM35_PRESET_FOR_ADC_TRIGGERING == ADC_s8SINGLE_CONVERSION)
		Local_u16ErrorStatus |= (ADC_enuDisableAutoTriggering()<<12);
#elif LM35_PRESET_FOR_ADC_TRIGGERING == ADC_u8Free_Running_Mode
		Local_u16ErrorStatus |= (ADC_enuEnableAutoTriggering(LM35_PRESET_FOR_ADC_TRIGGERING)<<12);
		Local_u16ErrorStatus |= (ADC_enuStartConversion()<<14);
#else
		Local_u16ErrorStatus |= (ADC_enuEnableAutoTriggering(LM35_PRESET_FOR_ADC_TRIGGERING)<<12);
#endif
		Local_u16ErrorStatus |= (ADC_enuEnableADC()<<10);

	}
	else
	{
		Local_enuErrorStatus = ES_OUT_OF_RANGE;
	}
	return Local_enuErrorStatus;
}


ES_t LM35_enuGetTemp(u16 * Copy_u16TempVal)
{
	ES_t Local_enuErrorStatus = ES_NOK;
	u8 Local_u8ErrorStatus;


#if LM35_PRESET_FOR_ADC_TRIGGERING == ADC_s8SINGLE_CONVERSION
		Local_u8ErrorStatus = ADC_enuStartConversion();
#endif


	if (LM35_u8Flag == 0)
	{
		*Copy_u16TempVal = ADC_u8Flag_Not_Raised;
	}
	else
	{
		/***************/
		u32 Local_u32ADC_step = ((u32)5000000 / 1024ul);
		u32 Local_u32ADC_Voltage;
		u16 Local_u16ADC_Reading;

		Local_u8ErrorStatus |=  (ADC_enuReadDataReg(&Local_u16ADC_Reading)<<2);

		Local_u32ADC_Voltage = Local_u16ADC_Reading * Local_u32ADC_step;
		*Copy_u16TempVal = Local_u32ADC_Voltage/ 10000ul;

		LM35_u8Flag = 0;
	}



	return Local_enuErrorStatus;
}

ES_t LM35_enuGetTempByPollying(u16 * Copy_u16TempVal)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADC_enuPollying ();
	Local_enuErrorStatus = ADC_enuReadDataReg(Copy_u16TempVal);


	return Local_enuErrorStatus;
}


