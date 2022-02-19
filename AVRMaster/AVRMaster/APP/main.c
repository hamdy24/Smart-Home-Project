/*
 * idle state
 * eeprom
 * detect keypad user
 * defrencing between promoted or remoted priorities
 * saeeed ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * */
#define F_CPU  16000000
#include "../LIBRARY/stdTypes.h"
#include "../LIBRARY/errorStates.h"
#include "../LIBRARY/AVR_REG.h"

#include "../MCAL/DIO/DIO_int.h"
//#include "../MCAL/TIMER1/Timer1_int.h"
#include "../MCAL/SPI/SPI_int.h"

#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"
#include "../HAL/EPROM3/EPROM_int.h"


#include "main_config.h"
#include "LoginSystem.h"

#include <util/delay.h>

int main (void)
{
/************************************************************************************************************/
/*******************************************INITIALIZATION***************************************************/
	//TWI_Init();
	//EEPROM_enuInit();
	DIO_enuInit();
//	Timer1_enuInit();
	SPI_vInitMaster();

	LCD_enuInit();
	Keypad_enuInit();
	
/***************************************************************************************************/
/* ***********************************************************************************************************/
/************************************************************************************************************/

	__asm("sei");// GIE

	u8 Program_Status_Flag;
	User_t LoginSystem_strAdmin;
	User_t LoginSystem_AstrUsers[MAX_NO_OF_USERS];
	u8 LoginSystem_NumOfRegisteredUsers=0;
	u8 LoginSystem_Au8Username[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
	u8 LoginSystem_Au8Password[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];

	u8 Keypad_Pressed_Key;
	u8 Current_State=0;
	EEPROM_ui8ReadByteFromAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,&Program_Status_Flag);
	_delay_ms(150);

	LCD_enuDisplayString("Welcome To Smart");
	LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
	LCD_enuDisplayString("Home !");
	_delay_ms (2000);
	LCD_enuClearDisplay();

	while (1)
	{

/************************************************************************************************************/
/******************************AIR CONDITIONER AUTOMATIC CONTROL*********************************************/

/*		u16 Local_u16TempVal;
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

			EEPROM_vWriteByteToAddress(NO_OF_REGISTERED_USERS_ADDRESS,LoginSystem_NumOfRegisteredUsers);
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
			LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_strAdmin.UserName, LoginSystem_strAdmin.Password);
			LoginSystem_strAdmin.User_Priority = ADMIN_PRIORITY;
			
		
			EEPROM_vWriteBlockToAddress(SAVED_ADMIN_INFO_ADDRESS ,LoginSystem_strAdmin.UserName , MAX_NO_OF_LETTERS);
			_delay_ms(200);
			EEPROM_vWriteBlockToAddress(SAVED_ADMIN_INFO_ADDRESS + MAX_NO_OF_LETTERS ,LoginSystem_strAdmin.Password , MAX_NO_OF_LETTERS);
			_delay_ms(200);
/*
			u8 Local_u8Iterator;
			for (Local_u8Iterator=0; Local_u8Iterator < MAX_NO_OF_LETTERS; Local_u8Iterator++)
			{

				EEPROM_vWriteByteToAddress(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator,LoginSystem_strAdmin.UserName[Local_u8Iterator]);
				_delay_ms(150);
				EEPROM_vWriteByteToAddress(SAVED_ADMIN_INFO_ADDRESS + Local_u8Iterator + MAX_NO_OF_LETTERS,LoginSystem_strAdmin.Password[Local_u8Iterator]);
				_delay_ms(150);
			}*/
			
			EEPROM_vWriteByteToAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,LOAD_LOGIN_SYSTEM_DATABASE);
			_delay_ms(150);

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
			EEPROM_ui8ReadByteFromAddress(NO_OF_REGISTERED_USERS_ADDRESS,&LoginSystem_NumOfRegisteredUsers);
			_delay_ms(200);
		

			EEPROM_vReadBlockFromAddress(SAVED_ADMIN_INFO_ADDRESS,LoginSystem_strAdmin.UserName,MAX_NO_OF_LETTERS);
			LoginSystem_strAdmin.UserName[MAX_NO_OF_LETTERS] = NULL_CHARACTER;
			_delay_ms(150);
			EEPROM_vReadBlockFromAddress(SAVED_ADMIN_INFO_ADDRESS + MAX_NO_OF_LETTERS,LoginSystem_strAdmin.Password,MAX_NO_OF_LETTERS);
			LoginSystem_strAdmin.Password[MAX_NO_OF_LETTERS] = NULL_CHARACTER;
			_delay_ms(150);
				
			LoginSystem_strAdmin.User_Priority=ADMIN_PRIORITY;
			_delay_ms(150);

			u16 Local_u16UserAddress;
			for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
			{
						Local_u16UserAddress = SAVED_USERS_INFO_ADDRESS +Local_u8Iterator*(2* MAX_NO_OF_LETTERS+1);
						EEPROM_vReadBlockFromAddress(Local_u16UserAddress,&LoginSystem_AstrUsers[Local_u8Iterator].UserName,MAX_NO_OF_LETTERS);
						_delay_ms(150);

						EEPROM_vReadBlockFromAddress(Local_u16UserAddress + MAX_NO_OF_LETTERS,&LoginSystem_AstrUsers[Local_u8Iterator].Password,MAX_NO_OF_LETTERS);
						_delay_ms(150);
						EEPROM_ui8ReadByteFromAddress(Local_u16UserAddress + 2*MAX_NO_OF_LETTERS,&LoginSystem_AstrUsers[Local_u8Iterator].User_Priority);
						_delay_ms(150);
						LoginSystem_AstrUsers[Local_u8Iterator].UserName[MAX_NO_OF_LETTERS] = NULL_CHARACTER;
						LoginSystem_AstrUsers[Local_u8Iterator].Password[MAX_NO_OF_LETTERS] = NULL_CHARACTER;
					}
				
			


			Program_Status_Flag = IDLE_STATUS;
		}
/*******************************************************************************************************************************************/

/************************************************************************************************************/
/************************************************************************************************************/


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
				Program_Status_Flag = ADMIN_LOGIN_PAGE_STATUS;
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

		if (Program_Status_Flag == ADMIN_LOGIN_PAGE_STATUS)
		{
			u8 LoginSystem_u8TrueFlag;
			u8 LoginSystem_u8TrialsLeft =3;
			LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_Au8Username, LoginSystem_Au8Password);


			LoginSystem_u8TrueFlag = (LoginSystem_u8Strcmp(LoginSystem_Au8Username,LoginSystem_strAdmin.UserName)) && ( LoginSystem_u8Strcmp(LoginSystem_Au8Password,LoginSystem_strAdmin.Password) ) ;
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
				Program_Status_Flag = ADMIN_MENU_STATUS;

			}
		}
		if (Program_Status_Flag == ADMIN_MENU_STATUS)
		{
			LCD_enuClearDisplay();
			LCD_enuDisplayString("Welcome Admin:");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString(LoginSystem_strAdmin.UserName);
			_delay_ms(500);
			LCD_enuClearDisplay();
			LCD_enuDisplayString("1-Registration");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("2-Lights 3-More");
			Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
			switch (Keypad_Pressed_Key)
			{
				case '1':
				if (LoginSystem_NumOfRegisteredUsers < MAX_NO_OF_USERS)
				{

					LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password);
					LCD_enuClearDisplay();
					LCD_enuDisplayString("1-Remoted");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("2-Nonremoted");
					Keypad_enuGetPressedKey(&Keypad_Pressed_Key);

					switch (Keypad_Pressed_Key)
					{
						case '1':
						LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = REMOTED_USER;
						break;
						case '2':
						LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = NORMAL_USER;
						break;

					}

					/**************Saving the new user into the EEPROM****************/
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + LoginSystem_NumOfRegisteredUsers*(2*MAX_NO_OF_LETTERS+1), LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName,MAX_NO_OF_LETTERS);
					_delay_ms(10);
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + LoginSystem_NumOfRegisteredUsers*(2*MAX_NO_OF_LETTERS+1) + MAX_NO_OF_LETTERS, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password,MAX_NO_OF_LETTERS);
					_delay_ms(10);
					LoginSystem_strAdmin.UserName[MAX_NO_OF_LETTERS] = NULL_CHARACTER;
					LoginSystem_strAdmin.Password[MAX_NO_OF_LETTERS] = NULL_CHARACTER;

					EEPROM_vWriteByteToAddress((SAVED_USERS_INFO_ADDRESS + LoginSystem_NumOfRegisteredUsers*(2*MAX_NO_OF_LETTERS+1) + 2*MAX_NO_OF_LETTERS),LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].User_Priority);
					_delay_ms(10);
					LoginSystem_NumOfRegisteredUsers++;

					EEPROM_vWriteByteToAddress(NO_OF_REGISTERED_USERS_ADDRESS,LoginSystem_NumOfRegisteredUsers);
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Registered");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Successfully");
					_delay_ms(700);
				}

				else
				{
					LCD_enuDisplayString("Max Limit");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("is Reached");
				}
				break;
				case '2':
				///////////////////////////////////
				break;
				case '3':

				LCD_enuClearDisplay();
				LCD_enuDisplayString("4-Promote User");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("5-Dimmer 6-Door");
				Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
				switch (Keypad_Pressed_Key)
				{
					case '4':
					//////////////
					break;
					case '5':
					/////////////
					break;
					case '6':
					////////////
					default:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Invalid Choice");
					_delay_ms(200);
				}
				break;

				default:
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Invalid Choice");
				_delay_ms(200);
			}

		}
		if (Program_Status_Flag == USER_LOGIN_PAGE_STATUS)///offline user should have interrupt
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
					_delay_ms(150);
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Remaining Trials: ");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayUnsignedInteger(LoginSystem_u8TrialsLeft);
					_delay_ms(150);
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

		if (Program_Status_Flag == USER_MENU_STATUS)//**offline user menu
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


