/*
 * LoginSystem.c
 *
 *  Created on: 15 Feb 2022
 *      Author: Dell
 */
#define F_CPU  16000000

#include "../LIBRARY/stdTypes.h"
#include "../LIBRARY/errorStates.h"

#include "../MCAL/DIO/DIO_int.h"
//#include "../MCAL/TIMER1/Timer1_int.h"
#include "../MCAL/UART/UART_interface.h"
#include "../HAL/EPROM3/EPROM_int.h"

//#include "../HAL/EEPROM/EEPROM_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"



#include "main_config.h"

#include <util/delay.h>


	ES_t LoginSystem_SaveNewUser(User_t Copy_strNewUser,u8 *Copy_u8NumofRegisteredUsers)
	{
		EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + (*Copy_u8NumofRegisteredUsers)*(2*MAX_NO_OF_LETTERS+1), Copy_strNewUser.UserName,MAX_NO_OF_LETTERS);
		_delay_ms(50);
		EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + (*Copy_u8NumofRegisteredUsers)*(2*MAX_NO_OF_LETTERS+1) + MAX_NO_OF_LETTERS, Copy_strNewUser.Password,MAX_NO_OF_LETTERS);
		_delay_ms(50);
		

		EEPROM_vWriteByteToAddress((SAVED_USERS_INFO_ADDRESS + (*Copy_u8NumofRegisteredUsers)*(2*MAX_NO_OF_LETTERS+1) + 2*MAX_NO_OF_LETTERS),Copy_strNewUser.User_Priority);
		_delay_ms(50);
		(*Copy_u8NumofRegisteredUsers)++;

		EEPROM_vWriteByteToAddress(NO_OF_REGISTERED_USERS_ADDRESS,(*Copy_u8NumofRegisteredUsers));
	}
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
		Copy_Au8Username[MAX_NO_OF_LETTERS] =NULL_CHARACTER;

		LCD_enuClearDisplay();
		LCD_enuDisplayString("Password:");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);

		for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
		{
			Keypad_enuGetPressedKey(&Local_u8PressedKey);
			LCD_enuSendData(Local_u8PressedKey);
			_delay_ms(500);
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0+Local_u8Iterator);
			LCD_enuSendData('*');
			Copy_Au8Password[Local_u8Iterator] = Local_u8PressedKey;

		}
		Copy_Au8Password[MAX_NO_OF_LETTERS] =NULL_CHARACTER;
		Local_enuErrorState = ES_OK;
		return Local_enuErrorState;
	}
	
		ES_t LoginSystem_enuGetDataFromUserBY_UART(u8 *Copy_Au8Username, u8 *Copy_Au8Password)
		{
			ES_t Local_enuErrorState = ES_NOK;
			UART_enuSendString("\r\n\r\nUsername: ");
			UART_enuRecieveString(Copy_Au8Username);
			Copy_Au8Username[MAX_NO_OF_LETTERS] =NULL_CHARACTER;
			UART_enuSendString("Password: ");
			UART_enuRecieveString(Copy_Au8Password);
			Copy_Au8Password[MAX_NO_OF_LETTERS] =NULL_CHARACTER;

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
	
	u8 LoginSystem_RemoveUser(User_t *Copy_AstrUsers,u8 *Copy_u8NumofRegisteredUsers)
		{
			u8 Local_Au8Username[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
			u8 Copy_u8Flag;
			UART_enuSendString("\r\nEnter Username Required to be Removed: ");
			UART_enuRecieveString(Local_Au8Username);
			Local_Au8Username[MAX_NO_OF_LETTERS] =NULL_CHARACTER;
			for (u8 Local_u8Iterator=0; Local_u8Iterator < (*Copy_u8NumofRegisteredUsers); Local_u8Iterator++)
			{
				Copy_u8Flag = LoginSystem_u8Strcmp (Copy_AstrUsers[Local_u8Iterator].UserName,Local_Au8Username);
				if (Copy_u8Flag )
				{
					Copy_AstrUsers[Local_u8Iterator]=Copy_AstrUsers[(*Copy_u8NumofRegisteredUsers)-1];
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + (Local_u8Iterator)*(2*MAX_NO_OF_LETTERS+1), Copy_AstrUsers[(*Copy_u8NumofRegisteredUsers)-1].UserName,MAX_NO_OF_LETTERS);
					_delay_ms(50);
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + (Local_u8Iterator)*(2*MAX_NO_OF_LETTERS+1) + MAX_NO_OF_LETTERS, Copy_AstrUsers[(*Copy_u8NumofRegisteredUsers)-1].Password,MAX_NO_OF_LETTERS);
					_delay_ms(50);
					EEPROM_vWriteByteToAddress((SAVED_USERS_INFO_ADDRESS + (Local_u8Iterator)*(2*MAX_NO_OF_LETTERS+1) + 2*MAX_NO_OF_LETTERS),Copy_AstrUsers[(*Copy_u8NumofRegisteredUsers)-1].User_Priority);
					_delay_ms(50);
					(*Copy_u8NumofRegisteredUsers)--;
					EEPROM_vWriteByteToAddress(NO_OF_REGISTERED_USERS_ADDRESS,(*Copy_u8NumofRegisteredUsers));
					_delay_ms(50);
					break;
				}
			}
			return Copy_u8Flag;
		}
	
/*
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

*/