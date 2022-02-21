/*
 * DC_Motor_program.c
 *
 *  Created on: Feb 17, 2022
 *      Author: hamdy
 */

#include "../../LIBRARIES/errorStates.h"
#include "../../LIBRARIES/stdTypes.h"
#include "../../MCAL/DIO/DIO_int.h"

#include "DC_Motor_private.h"
#include "DC_Motor_config.h"
ES_t DC_Motor_enuInit(u8 Copy_u8Motor1_State , u8 Copy_u8Motor2_State)
{
	ES_t Local_enuErorrState = ES_NOK;
	ES_t Local_enuErorrState_1 = ES_NOK;
	ES_t Local_enuErorrState_2 = ES_NOK;

	if(Copy_u8Motor1_State == CONNECTED)
	{
		DIO_enuSetPinDirection(MOTOR1_EN_PORT       , MOTOR1_EN_PIN       , DIO_u8OUTPUT);
		DIO_enuSetPinDirection(MOTOR1_INPUT_1_PORT  , MOTOR1_INPUT_1_PIN  , DIO_u8OUTPUT);
		DIO_enuSetPinDirection(MOTOR1_INPUT_2_PORT  , MOTOR1_INPUT_2_PIN  , DIO_u8OUTPUT);
		Local_enuErorrState_1 = ES_OK;
	}

	if(Copy_u8Motor2_State == CONNECTED)
	{
		DIO_enuSetPinDirection(MOTOR2_EN_PORT		, MOTOR2_EN_PIN		  , DIO_u8OUTPUT);
		DIO_enuSetPinDirection(MOTOR2_INPUT_3_PORT 	, MOTOR2_INPUT_3_PIN  , DIO_u8OUTPUT);
		DIO_enuSetPinDirection(MOTOR2_INPUT_4_PORT 	, MOTOR2_INPUT_4_PIN  , DIO_u8OUTPUT);
		Local_enuErorrState_2 = ES_OK;

	}

	Local_enuErorrState = Local_enuErorrState_1 & Local_enuErorrState_2;
	return Local_enuErorrState;
}

ES_t DC_Motor_enuStart(u8 Copy_u8MotorNum , u8 Copy_u8Motor_Direction)
{
	ES_t Local_enuErorrState = ES_NOK;

	if(Copy_u8MotorNum == 1)
	{
		DIO_enuSetPinValue(MOTOR1_EN_PORT,MOTOR1_EN_PIN,DIO_u8HIGH);

		if(Copy_u8Motor_Direction == CW)
		{
			DIO_enuSetPinValue(MOTOR1_INPUT_1_PORT  , MOTOR1_INPUT_1_PIN  , DIO_u8HIGH);
			DIO_enuSetPinValue(MOTOR1_INPUT_2_PORT  , MOTOR1_INPUT_2_PIN  , DIO_u8LOW);
			Local_enuErorrState =ES_OK;
		}

		else if(Copy_u8Motor_Direction == CCW)
		{
			DIO_enuSetPinValue(MOTOR1_INPUT_1_PORT  , MOTOR1_INPUT_1_PIN  , DIO_u8LOW);
			DIO_enuSetPinValue(MOTOR1_INPUT_2_PORT  , MOTOR1_INPUT_2_PIN  , DIO_u8HIGH);
			Local_enuErorrState =ES_OK;

		}
	}

	else if(Copy_u8MotorNum == 2)
	{
		DIO_enuSetPinValue(MOTOR2_EN_PORT,MOTOR2_EN_PIN,DIO_u8HIGH);
		if(Copy_u8Motor_Direction == CW)
		{
			DIO_enuSetPinValue(MOTOR2_INPUT_3_PORT 	, MOTOR2_INPUT_3_PIN  ,DIO_u8HIGH);
			DIO_enuSetPinValue(MOTOR2_INPUT_4_PORT 	, MOTOR2_INPUT_4_PIN  ,DIO_u8LOW);
			Local_enuErorrState =ES_OK;

		}

		else if(Copy_u8Motor_Direction == CCW)
		{
			DIO_enuSetPinValue(MOTOR2_INPUT_3_PORT 	, MOTOR2_INPUT_3_PIN  ,DIO_u8LOW);
			DIO_enuSetPinValue(MOTOR2_INPUT_4_PORT 	, MOTOR2_INPUT_4_PIN  ,DIO_u8HIGH);
			Local_enuErorrState =ES_OK;

		}
	}
	return Local_enuErorrState;
}

ES_t DC_Motor_enuStop(u8 Copy_u8MotorNum)
{
	ES_t Local_enuErorrState = ES_NOK;

	if(Copy_u8MotorNum == 1)
	{
		DIO_enuSetPinValue(MOTOR1_EN_PORT,MOTOR1_EN_PIN,DIO_u8LOW);
		Local_enuErorrState =ES_OK;

	}

	else if(Copy_u8MotorNum == 2)
	{
		DIO_enuSetPinValue(MOTOR2_EN_PORT,MOTOR2_EN_PIN,DIO_u8LOW);
		Local_enuErorrState =ES_OK;
}
	return Local_enuErorrState;
}
