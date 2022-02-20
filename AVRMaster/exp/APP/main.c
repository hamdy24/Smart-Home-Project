/*
 * exp.c
 *
 * Created: 20/02/2022 17:31:46
 * Author : Dell
 */ 


#define F_CPU  16000000
#include "../LIBRARY/stdTypes.h"
#include "../LIBRARY/errorStates.h"

#include "../MCAL/DIO/DIO_int.h"

#include "../MCAL/EXTI/EXTI_config.h"
#include "../MCAL/EXTI/EXTI_int.h"

#include "../HAL/EPROM3/EPROM_int.h"

#include "../HAL/LCD/LCD_int.h"

#include <util/delay.h>

#include "main_config.h"

//extern EXTI_T EXTI_enuAstrEXTI_CONFIG[INTERRUPT_NUM];
u8 LoginSystem_u8AdminOnlineFlag = 0;
u8 LoginSystem_u8PromotedUserOnlineFlag = 0;
u8 ShowToUser = 0;
u8 Program_Status_Flag = 0;
volatile void Button_Pressed_Interrupt()
{
	if (LoginSystem_u8AdminOnlineFlag)	//5od karar hna
	{
		
	}
	else if	(LoginSystem_u8PromotedUserOnlineFlag)
	{
		
	}
	else
	{
		Program_Status_Flag = USER_LOGIN_PAGE_STATUS;
		ShowToUser = MAIN_MENU;
	}
	
}

int main(void)
{
	u8 Current_State,LoginSystem_u8TrialsLeft =3;
    //DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN6 , DIO_u8OUTPUT);
	DIO_enuInit();



LCD_enuInit();

DIO_enuSetPinDirection(DIO_u8GROUP_D,DIO_u8PIN1,DIO_u8OUTPUT);
   // EXTI_enuInit(EXTI_enuAstrEXTI_CONFIG);
	EXTI_enuInit();
	__asm("sei");// GIE
	EXTI_enuCallBack((volatile void (*) (void))Button_Pressed_Interrupt);// , 0);
	EEPROM_ui8ReadByteFromAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,&Program_Status_Flag);
	_delay_ms(150);

	LCD_enuDisplayString("Welcome To Smart");
	LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
	LCD_enuDisplayString("Home !");
	_delay_ms (2000);
	LCD_enuClearDisplay();
	if (Program_Status_Flag == FIRST_TIME_USE_STATUS)
	{
		_delay_ms(150);
		LCD_enuDisplayString("For First Time");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
		LCD_enuDisplayString("Use");
		_delay_ms(2000);
		LCD_enuClearDisplay();
		LCD_enuDisplayString("Register Admin");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
		LCD_enuDisplayString("on Keypad");
		_delay_ms(2000);
		EEPROM_vWriteByteToAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,LOAD_LOGIN_SYSTEM_DATABASE);
		_delay_ms(150);

		LCD_enuClearDisplay();
		LCD_enuDisplayString("Saved");
		LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
		LCD_enuDisplayString("Successfully");
		_delay_ms(1000);
		LCD_enuClearDisplay();
		Program_Status_Flag = IDLE_STATUS;
		}
		else if (Program_Status_Flag == LOAD_LOGIN_SYSTEM_DATABASE)
		{
			LCD_enuClearDisplay();
			LCD_enuDisplayString("LOADING");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("Successfully");
			_delay_ms(1000);
			Program_Status_Flag = IDLE_STATUS;
		}
		
    while (1) 
    {
		while (Program_Status_Flag == IDLE_STATUS)
		{

			//LCD_enuClearDisplay();
			
			switch (Current_State)
			{
				
				case State_ROOM_1:
				LCD_enuDisplayString("Room 1 controlled");
				break;
				case State_ROOM_2:
				LCD_enuDisplayString("Room 2 controlled");
				break;
				case State_ROOM_3:
				LCD_enuDisplayString("Room 3 controlled");
				break;
				case State_ROOM_4:
				LCD_enuDisplayString("Room 4 controlled");
				break;
				case State_ROOM_5:
				LCD_enuDisplayString("Room 5 controlled");
				break;
				case State_Air_Cond:
				LCD_enuDisplayString("Air Cond controlled");
				break;
				case State_Door:
				LCD_enuDisplayString("Door controlled");
				break;
				case State_Dimmer:
				LCD_enuDisplayString("Dimmer controlled");
				break;
				default:
				LCD_enuClearDisplay();
				LCD_enuDisplayString("All Devices");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("Are Off");
				break;
				
			}
    }
			while (Program_Status_Flag == USER_LOGIN_PAGE_STATUS)///off line user should have interrupt
			{

				u8 LoginSystem_u8TrueFlag;
				//	u8 LoginSystem_u8TrialsLeft =3;

				
				/*********************Search The Users Array***********************/
			

				
					
					LCD_enuClearDisplay();
					LCD_enuDisplayString("User Login");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Page");
					_delay_ms(1500);
				
					if (LoginSystem_u8TrialsLeft == 0)
					{
						Program_Status_Flag = BLOCKING_STATUS;
					}
				

			}
}
}

