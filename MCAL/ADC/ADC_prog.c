/*
 * ADC_prog.c
 *
 *  Created on: 25 Oct 2021
 *      Author: Dell
 */

#include "../../LIBRARIES/errorStates.h"
#include "../../LIBRARIES/stdTypes.h"
#include "../ADC/ADC_config.h"
#include "../ADC/ADC_priv.h"

#include "../../LIBRARIES/interrupt.h"

void (* ADC_pfunGlobalPtr)(void) = NULL;

ES_t ADC_enuInit (void)
{
	ES_t Local_enuErrorStatus = ES_NOK;
	/*********************Boundary Check*********************/
	if ((ADC_u8REFERENCE_VOLTAGE <= ADC_u8INTERNAL_REF) && (ADC_u8DATA_REG_ADJUSTMENT <= ADC_u8LEFT_ADJUSMENT) &&
			(ADC_u8CHANNEL <= ADC_u8DIFF_CH_ADC4_ADC2_1x) && (ADC_u8PRESCALAR <= ADC_u8PRESC128) &&
			(ADC_u8TRIGGERING_MODE <= ADC_u8Timer1_Capture_Event) && (ADC_u8INTERRUPT_STATE <= ADC_u8INTERRUPT_ON) &&
			(ADC_u8STATE <= ADC_u8ENABLED))
	{
			ADMUX |= (ADC_u8REFERENCE_VOLTAGE<<REF_S0);

			ADMUX |= (ADC_u8DATA_REG_ADJUSTMENT<<ADLAR);

			ADMUX |= (ADC_u8CHANNEL<<MUX0);

			ADCSRA |= (ADC_u8PRESCALAR<<ADPS0);

		#if ADC_u8TRIGGERING_MODE == ADC_s8SINGLE_CONVERSION
			ADCSRA &=~	(1<<ADATE);		//line maloosh lazma
		#else
			SFIOR |= (ADC_u8TRIGGERING_MODE << ADTS0);
			ADCSRA |= (1<<ADATE);
		#endif

			ADCSRA |=	(ADC_u8INTERRUPT_STATE<<ADIE);

			ADCSRA |=	(ADC_u8STATE<<ADEN);

			Local_enuErrorStatus = ES_OK;
	}
	else
	{
		Local_enuErrorStatus = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorStatus;
}

ES_t ADC_enuStartConversion (void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADCSRA |= (1<<ADSC);

	Local_enuErrorStatus = ES_OK;
	return Local_enuErrorStatus;
}

ES_t ADC_enuChooseChannel(u8 Copy_u8ChannelID)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	if (Copy_u8ChannelID <= ADC_u8DIFF_CH_ADC4_ADC2_1x)
	{
		ADMUX &=~ (0b11111<<MUX0);
		ADMUX |= (Copy_u8ChannelID<<MUX0);

		Local_enuErrorStatus = ES_OK;

	}
	else
	{
		Local_enuErrorStatus = ES_OUT_OF_RANGE;
	}
	return Local_enuErrorStatus;
}

ES_t ADC_enuEnableAutoTriggering (u8 Copy_u8TriggeringMode)
{
	ES_t Local_enuErrorStatus = ES_NOK;
	if (Copy_u8TriggeringMode <= ADC_u8Timer1_Capture_Event)
	{
		ADCSRA &=~ (1<<ADATE);
		SFIOR &=~ (0b111 << ADTS0);

		SFIOR |= (Copy_u8TriggeringMode << ADTS0);
		ADCSRA |= (1<<ADATE);

		Local_enuErrorStatus = ES_OK;
	}
	else
	{
		Local_enuErrorStatus = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorStatus;
}

ES_t ADC_enuDisableAutoTriggering (void)
{
	ES_t Local_enuErrorStatus = ES_NOK;
	ADCSRA &=~ (1<<ADATE);

	Local_enuErrorStatus = ES_OK;
	return Local_enuErrorStatus;
}

ES_t ADC_enuPollying(void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	if ( ((ADCSRA>>ADEN) & 1) == ADC_u8ENABLED)
	{
		while ( ((ADCSRA>>ADIF) & 1) == ADC_u8Flag_Not_Raised);

		ADCSRA |= (1<<ADIF);

		Local_enuErrorStatus = ES_OK;
	}


	return Local_enuErrorStatus;
}

ES_t ADC_enuEnableADC_Inturrupt(void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADCSRA |=	(1<<ADIE);


	Local_enuErrorStatus = ES_OK;
	return Local_enuErrorStatus;
}

ES_t ADC_enuDisableADC_Inturrupt(void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADCSRA &=~	(1<<ADIE);


	Local_enuErrorStatus = ES_OK;

	return Local_enuErrorStatus;
}

ES_t ADC_enuEnableADC(void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADCSRA |=	(1<<ADEN);

	Local_enuErrorStatus = ES_OK;
	return Local_enuErrorStatus;
}

ES_t ADC_eniDisableADC(void)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	ADCSRA &=~	(1<<ADEN);

	Local_enuErrorStatus = ES_OK;

	return Local_enuErrorStatus;
}

ES_t ADC_enuReadDataReg (u16 * Copy_pu16ReadValue)
{
	ES_t Local_enuErrorStatus = ES_NOK;

	if (Copy_pu16ReadValue != NULL)
	{
#if ADC_u8DATA_REG_ADJUSTMENT == ADC_u8RIGHT_ADJUSMENT
	*Copy_pu16ReadValue = ADCL;
	*Copy_pu16ReadValue |= (u16)(ADCH<<8);
#elif ADC_u8DATA_REG_ADJUSTMENT == ADC_u8LEFT_ADJUSMENT
	*Copy_pu16ReadValue = (ADCL>>6);
	*Copy_pu16ReadValue |= (u16)(ADCH<<2);
#endif

	Local_enuErrorStatus = ES_OK;
	}
	else
	{
		Local_enuErrorStatus = 	ES_NULL_POINTER;
	}



	return Local_enuErrorStatus;
}

ES_t ADC_enuReadHigherDataReg (u8 * Copy_pu8HigherReg)
{
	ES_t Local_enuErrorStatus = ES_NOK;
	if (Copy_pu8HigherReg != NULL)
	{
#if ADC_u8DATA_REG_ADJUSTMENT == ADC_u8RIGHT_ADJUSMENT
	*Copy_pu8HigherReg = (ADCL>>2);
	*Copy_pu8HigherReg |= (ADCH<<6);
#elif ADC_u8DATA_REG_ADJUSTMENT == ADC_u8LEFT_ADJUSMENT
	*Copy_pu8HigherReg = ADCH;

#endif

	Local_enuErrorStatus = ES_OK;

	}
	else
	{
		Local_enuErrorStatus = 	ES_NULL_POINTER;
	}




	return Local_enuErrorStatus;
}

ES_t ADC_enuCallBack (volatile void (* Copy_pfun)(void))
{
	ES_t Local_enuErrorStatus = ES_NOK;
	if (Copy_pfun != NULL)
	{
		ADC_pfunGlobalPtr = Copy_pfun;
		Local_enuErrorStatus = 	ES_OK;
	}
	else
	{
		Local_enuErrorStatus = 	ES_NULL_POINTER;
	}


	return Local_enuErrorStatus;
}


ISR (VECT_ADC)
{
	if (ADC_pfunGlobalPtr != NULL)
	{
		ADC_pfunGlobalPtr();
	}
}
