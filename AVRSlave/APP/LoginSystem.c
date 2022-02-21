/*
 * LoginSystem.c
 *
 *  Created on: 15 Feb 2022
 *      Author: Dell
 */
#include "../LIBRARIES/stdTypes.h"
#include "../LIBRARIES/errorStates.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/TIMER1/Timer1_int.h"

#include "main_config.h"

#include <util/delay.h>

	void ServoMotor_Door(u8 Copy_u8Angle)
	{
		float Local_floatDuty=0;
		Local_floatDuty = (((float)Copy_u8Angle/180.0)*6)+5;
		Timer1_enuPWM(50,Local_floatDuty);
	}

	void OPEN_LEDS(u8 freq,u8 light)
	{
		DIO_enuSetPinValue(DIO_u8GROUP_B,DIO_u8PIN6,DIO_u8HIGH);
		Timer1_enuPWM(freq,light);
	}

	void CLOSE_LEDS()
	{
		DIO_enuSetPinValue(DIO_u8GROUP_B,DIO_u8PIN6,DIO_u8LOW);
	}

