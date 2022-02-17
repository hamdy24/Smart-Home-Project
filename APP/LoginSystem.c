/*
 * LoginSystem.c
 *
 *  Created on: 15 Feb 2022
 *      Author: Dell
 */
#include "../LIBRARIES/stdTypes.h"
#include "../LIBRARIES/errorStates.h"

#include "../MCAL/UART/UART_interface.h"

#include "../HAL/EEPROM/EEPROM_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"
#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/TIMER1/Timer1_int.h"

#include "main_config.h"

#include <util/delay.h>

	ES_t LoginSystem_enuGetDataFromUserByKeypad(u8 *Copy_Au8Username, u8 *Copy_Au8Password)
	{
		ES_t Local_enuErrorState = ES_NOK;
		u8 Local_u8PressedKey;
		u8 Local_u8Iterator;
		LCD_enuClearDisplay();
		LCD_enuDisplayString("Username:");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
		for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
		{
			Keypad_enuGetPressedKey(&Local_u8PressedKey);
			LCD_enuSendData(Local_u8PressedKey);
			Copy_Au8Username[Local_u8Iterator] = Local_u8PressedKey;
		}
		Copy_Au8Username[Local_u8Iterator] =NULL_CHARACTER;

		LCD_enuClearDisplay();
		LCD_enuDisplayString("Password:");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);

		for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
		{
			Keypad_enuGetPressedKey(&Local_u8PressedKey);
			LCD_enuSendData(Local_u8PressedKey);
			_delay_ms(200);
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0+Local_u8Iterator);
			LCD_enuSendData('*');
			Copy_Au8Password[Local_u8Iterator] = Local_u8PressedKey;
		}
		Copy_Au8Password[Local_u8Iterator] =NULL_CHARACTER;
		Local_enuErrorState = ES_OK;
		return Local_enuErrorState;
	}

	ES_t LoginSystem_enuGetDataFromUserBY_UART(u8 *Copy_Au8Username[22], u8 *Copy_Au8Password)
		{
			ES_t Local_enuErrorState = ES_NOK;
			UART_enuSendString("Username: ");
			UART_enuRecieveString(Copy_Au8Username);

			UART_enuSendString("Password: ");
			UART_enuRecieveString(Copy_Au8Password);

			Local_enuErrorState = ES_OK;
			return Local_enuErrorState;
		}

	u8 LoginSystem_u8Strcmp (char *Copy_Au8Sring1, char *Copy_Au8Sring2)
	{
		u8 Local_u8Iterator=0;
		while(Copy_Au8Sring1[Local_u8Iterator]==Copy_Au8Sring2[Local_u8Iterator])
		{
			if (Copy_Au8Sring1[Local_u8Iterator]==NULL_CHARACTER || Copy_Au8Sring2[Local_u8Iterator]==NULL_CHARACTER)
				break;
			Local_u8Iterator++;
		}

		if (Copy_Au8Sring1[Local_u8Iterator]==NULL_CHARACTER && Copy_Au8Sring2[Local_u8Iterator]==NULL_CHARACTER)
			return TRUE;
		else
			return FALSE;
	}

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

