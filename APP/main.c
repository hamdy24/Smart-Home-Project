/*
 * idle state
 * detect keypad user
 * defrencing between promoted or remoted priorities
 * saeeed ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * */

#include "../LIBRARIES/stdTypes.h"
#include "../LIBRARIES/errorStates.h"

#include "../LIBRARIES/AVR_REG.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/ADC/ADC_int.h"
#include "../MCAL/IIC/IIC_int.h"
#include "../MCAL/UART/UART_interface.h"
#include "../MCAL/TIMER1/Timer1_int.h"

#include "../HAL/EEPROM/EEPROM_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"
#include "../HAL/DC_Motor/DC_Motor_interface.h"
#include "../HAL/LM35/LM35_int.h"

#include "main_config.h"
#include "LoginSystem.h"

#include <util/delay.h>



int main (void)
{
/************************************************************************************************************/
/*******************************************INITIALIZATION***************************************************/
	TWI_Init();
	LCD_enuInit();
	Keypad_enuInit();
	DIO_enuInit();
	Timer1_enuInit();
	UART_enuUART_Init();
	LM35_enuInit();
	DC_Motor_enuInit(1,0);

/*************************************************************************************************************/
/* 																											 */
/* ***********************************************************************************************************/
/************************************************************************************************************/

	DIO_enuSetPinDirection(DIO_u8GROUP_D,DIO_u8PIN1,DIO_u8OUTPUT);
	__asm("sei");// GIE


	u8 Program_Status_Flag;
	User_t LoginSystem_strAdmin;
	User_t LoginSystem_AstrUsers[MAX_NO_OF_USERS];
	u8 LoginSystem_NumOfRegisteredUsers=0;
	u8 LoginSystem_Au8Username[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
	u8 LoginSystem_Au8Password[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];

	u8 Keypad_Pressed_Key;

	EEPROM_Read(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,&Program_Status_Flag);
	Program_Status_Flag = FIRST_TIME_USE_STATUS;

	LCD_enuDisplayString("Welcome To Smart");
	UART_enuSendString("Welcome To Smart Home !\r\n ");
	LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
	LCD_enuDisplayString("Home !");
	_delay_ms (2000);

	LCD_enuClearDisplay();




	while (1)
	{

/************************************************************************************************************/
/******************************AIR CONDITIONER AUTOMATIC CONTROL*********************************************/
/*
		u16 Local_u16TempVal;
		LM35_enuGetTemp(&Local_u16TempVal);

		u8 Local_High_temp_limit = 28 ;
		u8 Local_Low_temp_limit = 21 ;

		if (Local_u16TempVal != ADC_u8Flag_Not_Raised)
		{
			if(Local_u16TempVal > Local_High_temp_limit)
			{
				DC_Motor_enuStart(1,1);
			}
			else if(Local_u16TempVal < Local_Low_temp_limit)
			{
				DC_Motor_enuStop(1);
			}

		}
*/


/***************************************************************************************************************/
/***************************************************************************************************************/
		if (Program_Status_Flag == FIRST_TIME_USE_STATUS)
		{
			LCD_enuDisplayString("For CHECK Time");

			EEPROM_Write(NO_OF_REGISTERED_USERS_ADDRESS,LoginSystem_NumOfRegisteredUsers);
			LCD_enuDisplayString("For First Time");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("Use");
			_delay_ms(1000);
			LCD_enuClearDisplay();
			LCD_enuDisplayString("Register Admin");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("on Keypad");
			_delay_ms(1000);
			LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_strAdmin.UserName, LoginSystem_strAdmin.Password);
			LoginSystem_strAdmin.User_Priority = ADMIN_PRIORITY;


			EEPROM_Write(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,LOAD_LOGIN_SYSTEM_DATABASE);
			_delay_ms(150);
			u8 Local_u8Iterator;
			for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
			{

				EEPROM_Write(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator,LoginSystem_strAdmin.UserName[Local_u8Iterator]);
				_delay_ms(150);
				EEPROM_Write(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator + MAX_NO_OF_LETTERS,LoginSystem_strAdmin.Password[Local_u8Iterator]);
				_delay_ms(150);
			}
			EEPROM_Write(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator + MAX_NO_OF_LETTERS,LoginSystem_strAdmin.User_Priority);

			LCD_enuClearDisplay();
			LCD_enuDisplayString("Saved");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("Successfully");
			_delay_ms(1000);
			Program_Status_Flag = IDLE_STATUS;

		}

/************************************************************************************************************/
/************************************************************************************************************/

		else if (Program_Status_Flag == LOAD_LOGIN_SYSTEM_DATABASE)
		{
			/****************************************Load the Login System database from the EEPROM***************************************/

			EEPROM_Read(NO_OF_REGISTERED_USERS_ADDRESS,&LoginSystem_NumOfRegisteredUsers);
			UART_enuSendString("LOAD_LOGIN_SYSTEM_DATABASE");
			for (u8 Local_u8Iterator=0; Local_u8Iterator <= MAX_NO_OF_LETTERS; Local_u8Iterator++)
			{
				if (Local_u8Iterator < MAX_NO_OF_LETTERS)
				{
					EEPROM_Read(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator,(LoginSystem_strAdmin.UserName+Local_u8Iterator));
					EEPROM_Read(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator + MAX_NO_OF_LETTERS,(LoginSystem_strAdmin.Password+Local_u8Iterator));
				}
				else
				{
					//EEPROM_Read(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator + MAX_NO_OF_LETTERS,&LoginSystem_strAdmin.User_Priority);
					LoginSystem_strAdmin.UserName[Local_u8Iterator] = NULL_CHARACTER;
					LoginSystem_strAdmin.Password[Local_u8Iterator] = NULL_CHARACTER;
				}

			}
			u16 Local_u16UserAddress;
			for (u8 Local_u8Iterator1=0; Local_u8Iterator1 < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator1++)
			{
				for (u8 Local_u8Iterator2=0; Local_u8Iterator2 <= MAX_NO_OF_LETTERS; Local_u8Iterator2++)
				{
					if (Local_u8Iterator2 < MAX_NO_OF_LETTERS)
					{
						Local_u16UserAddress = SAVED_USERS_INFO_ADDRESS + Local_u8Iterator2 + Local_u8Iterator1*(2* MAX_NO_OF_LETTERS+1);
						EEPROM_Read(Local_u16UserAddress,&LoginSystem_AstrUsers[Local_u8Iterator1].UserName[Local_u8Iterator2]);
						EEPROM_Read(Local_u16UserAddress + MAX_NO_OF_LETTERS,&LoginSystem_AstrUsers[Local_u8Iterator1].Password[Local_u8Iterator2]);
					}
					else
					{
						EEPROM_Read(Local_u16UserAddress + MAX_NO_OF_LETTERS,&LoginSystem_AstrUsers[Local_u8Iterator1].User_Priority);
						LoginSystem_AstrUsers[Local_u8Iterator1].UserName[Local_u8Iterator2] = NULL_CHARACTER;
						LoginSystem_AstrUsers[Local_u8Iterator1].Password[Local_u8Iterator2] = NULL_CHARACTER;
					}
				}
			}


			Program_Status_Flag = IDLE_STATUS;
		}
/*******************************************************************************************************************************************/


		UART_enuSendString("PRESS ENTER TO START\r\n");

		u8 Local_u8Data[22];
		UART_enuRecieveString(Local_u8Data);
		if(UART_enuCheck_Connection())
		{
				Program_Status_Flag = ADMIN_LOGIN_PAGE_STATUS;
		}

/************************************************************************************************************/
/************************************************************************************************************/

/*
		if (Program_Status_Flag == IDLE_STATUS)
		{

			LCD_enuClearDisplay();
			LCD_enuDisplayString("Choose mode");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("1-Admin  2-User");
			Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
			switch (Keypad_Pressed_Key)
			{
			case '1':

				break;
			case '2':
				Program_Status_Flag = USER_LOGIN_PAGE_STATUS;
				break;
			default:
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Invalid Choice");
				_delay_ms(200);
			}

		}
*/

/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == ADMIN_LOGIN_PAGE_STATUS)
		{
			UART_enuSendString("ADMIN_LOGIN_PAGE_STATUS");

			u8 LoginSystem_u8_AdminTrueFlag;
			u8 LoginSystem_u8_Remoted_USERTrueFlag;
			u8 LoginSystem_u8TrialsLeft =3;
			LoginSystem_enuGetDataFromUserBY_UART(LoginSystem_Au8Username, LoginSystem_Au8Password);


			LoginSystem_u8_AdminTrueFlag = (LoginSystem_u8Strcmp(LoginSystem_Au8Username,LoginSystem_strAdmin.UserName)) && ( LoginSystem_u8Strcmp(LoginSystem_Au8Password,LoginSystem_strAdmin.Password) ) ;

			if(LoginSystem_u8_AdminTrueFlag == TRUE)
			{
				Program_Status_Flag = ADMIN_MENU_STATUS;
			}
			else
			{
				for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
				{
					LoginSystem_u8_Remoted_USERTrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
					if (LoginSystem_u8_Remoted_USERTrueFlag == TRUE)
					{
						if(LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == PROMOTED_USER ) // pro>>>>   // rem: key or not
							Program_Status_Flag = REMOTED_USER_MENU_STATUS;
						break;
					  }
					}

				}


			if (LoginSystem_u8_AdminTrueFlag == FALSE && LoginSystem_u8_Remoted_USERTrueFlag ==FALSE )
			{
				LoginSystem_u8TrialsLeft--;
				UART_enuSendString("\r\nWrong Username or Password");
				_delay_ms(300);
				UART_enuSendString("\r\nRemaining Trials: ");
				UART_enuSendChar('0'+LoginSystem_u8TrialsLeft);
				_delay_ms(1000);
				if (LoginSystem_u8TrialsLeft == 0)
				{
					Program_Status_Flag = BLOCKING_STATUS;
				}
			}
			else
			{
				UART_enuSendString("\r\nLogged in Successfully");
				_delay_ms(500);
			}
		}

/************************************************************************************************************/
/************************************************************************************************************/

/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == ADMIN_MENU_STATUS)
			{
				UART_enuSendString("\r\nWelcome Admin:\n");
				UART_enuSendString(LoginSystem_strAdmin.UserName);
				_delay_ms(500);

				u8 Valid_Choice_Flag=0;
			while(!Valid_Choice_Flag)
				{
				UART_enuSendString("\r\n1-Add User");
				UART_enuSendString("\r\n2-Remove User");
				UART_enuSendString("\r\n3-Lights");
				UART_enuSendString("\r\n4-Door");
				UART_enuSendString("\r\n5-Air Conditioner\r\n");
				u8 UART_CHOICE;
				UART_enuRecieveChar(&UART_CHOICE);
				switch (UART_CHOICE)
				{
				case '1':
					if (LoginSystem_NumOfRegisteredUsers < MAX_NO_OF_USERS)
					{
						u8 Valid_Choice_Flag=0;
						while(!Valid_Choice_Flag)
						{
						LoginSystem_enuGetDataFromUserBY_UART(LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password);
						UART_enuSendString("\r\n1-Remoted");
						UART_enuSendString("\r\n2-Promoted");
						UART_enuSendString("\r\n3-Nonremoted");
						UART_enuSendString("\r\nPRIORITY->Remote'Promoted'->\r\nNonremoted(Keypad ONLY)->\r\nRemoted");
						UART_enuRecieveChar(&UART_CHOICE);

						switch (UART_CHOICE)
						{
						case '1':
							LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = REMOTED_USER;
							Valid_Choice_Flag++;
							break;
						case '2':
							LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = PROMOTED_USER;
							Valid_Choice_Flag++;
							break;
						case '3':
							LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = NORMAL_USER;
							Valid_Choice_Flag++;
							break;
						default:
							UART_enuSendString("\r\nINVALID CHOICE");
						}
						}
						/**************Saving the new user into the EEPROM****************/
						u16 Local_u16UserAddress;
						u8 Local_u8Iterator;
						for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
						{
								Local_u16UserAddress = SAVED_USERS_INFO_ADDRESS + Local_u8Iterator + LoginSystem_NumOfRegisteredUsers*(2* MAX_NO_OF_LETTERS+1);
								EEPROM_Write(Local_u16UserAddress,LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName[Local_u8Iterator]);
								_delay_ms(50);
								EEPROM_Write(Local_u16UserAddress + MAX_NO_OF_LETTERS,LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password[Local_u8Iterator]);
								_delay_ms(50);
						}
						Local_u16UserAddress = SAVED_USERS_INFO_ADDRESS + Local_u8Iterator + LoginSystem_NumOfRegisteredUsers*(2* MAX_NO_OF_LETTERS+1);
						EEPROM_Write(Local_u16UserAddress + MAX_NO_OF_LETTERS,LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].User_Priority);
						_delay_ms(50);
						LoginSystem_NumOfRegisteredUsers++;
						EEPROM_Write(NO_OF_REGISTERED_USERS_ADDRESS,LoginSystem_NumOfRegisteredUsers);

						UART_enuSendString("\r\nRegistered Successfully");
						_delay_ms(700);
					}

					else
					{
						UART_enuSendString("\r\nMax Limit is Reached");
					}
					Valid_Choice_Flag++;
					break;
				case '2'://remove user

					Valid_Choice_Flag++;
					break;
				case '3'://lights


					switch (Keypad_Pressed_Key)
					{
					case '1':
						//////////////
						break;
					case '2':
						/////////////
						break;
					default:
					{
					UART_enuSendString("\r\nInvalid Choice");
					_delay_ms(200);
					break;
					}
				}
					Valid_Choice_Flag++;
					break;


				case '4'://door

					UART_enuSendString("\r\n1-Open   \r\n2-Close");
					UART_enuRecieveChar(&UART_CHOICE);
					if(UART_CHOICE == 1)
					{
						ServoMotor_Door(180);
					}
					else
					{
						ServoMotor_Door(0);
					}

					Valid_Choice_Flag++;
						break;

				case '5':// air cond immediat control

						UART_enuSendString("\r\n1-Turn ON\r\n2-Turn OFF\r\n");
						UART_enuRecieveChar(&UART_CHOICE);
						if(UART_CHOICE == '1')
						{
							DC_Motor_enuStart(1,1);
						}
						else
						{
							DC_Motor_enuStop(1);
						}

					break;

				default:
					UART_enuSendString("\r\nInvalid Choice");
					_delay_ms(200);
				}
					}
			Program_Status_Flag = IDLE_STATUS;

			}

/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == REMOTED_USER_MENU_STATUS)
		{

			UART_enuSendString("\r\nWelcome User:");
			_delay_ms(500);

			u8 Valid_Choice_Flag=0;
		while(!Valid_Choice_Flag)
			{
			UART_enuSendString("\r\n1-Lights");
			u8 UART_CHOICE;
			UART_enuRecieveChar(&UART_CHOICE);
			switch (UART_CHOICE)
			{

			case '1'://lights

				switch (Keypad_Pressed_Key)
				{
				case '1':
					//////////////
					break;
				case '2':
					/////////////
					break;
				default:
				{
				UART_enuSendString("\r\nInvalid Choice");
				_delay_ms(200);
				break;
				}
			}
				Valid_Choice_Flag++;
				break;

			default:
				UART_enuSendString("\r\nInvalid Choice");
				_delay_ms(200);
			}
				}
		Program_Status_Flag = IDLE_STATUS;
}

/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == USER_LOGIN_PAGE_STATUS)
			{

				u8 LoginSystem_u8TrueFlag;
				u8 LoginSystem_u8TrialsLeft =3;

				LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_Au8Username, LoginSystem_Au8Password);
				/*********************Search The Users Array***********************/
				for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
				{
					LoginSystem_u8TrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
					if (LoginSystem_u8TrueFlag == TRUE)
					{
						break;
					}
				}

				if (LoginSystem_u8TrueFlag == FALSE)
				{
					LoginSystem_u8TrialsLeft--;
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Wrong Username");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("or Password");
					_delay_ms(1000);
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Remaining Trials: ");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayUnsignedInteger(LoginSystem_u8TrialsLeft);
					_delay_ms(1000);
					if (LoginSystem_u8TrialsLeft == 0)
					{
						Program_Status_Flag = BLOCKING_STATUS;
					}
				}
				else
				{
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Logged in");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Successfully");
					_delay_ms(700);
					Program_Status_Flag = USER_MENU_STATUS;

			}

		}

/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == USER_MENU_STATUS)
		{
							LCD_enuClearDisplay();
							LCD_enuDisplayString("Welcome User");
							LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
							LCD_enuDisplayString(LoginSystem_strAdmin.UserName);
							_delay_ms(500);
							LCD_enuClearDisplay();
							LCD_enuDisplayString("1-Lights 2-Dimmer");
							LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
							LCD_enuDisplayString("3-Air Cond");
							Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
							switch (Keypad_Pressed_Key)
							{
							case '1':
							//////////////////
								break;
							case '2':
								///////////////////////////////////
								break;
							case '3':
								//////////////
								break;


							default:
								LCD_enuClearDisplay();
								LCD_enuDisplayString("Invalid Choice");
								_delay_ms(200);
								break;
							}
							Program_Status_Flag = IDLE_STATUS;
						}

	}



	return 0;
}


