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
#include "../MCAL/UART/UART_interface.h"
#include "../HAL/EPROM3/EPROM_int.h"

#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"



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
	UART_enuUART_Init();
	
	
	LCD_enuInit();
	Keypad_enuInit();
	DIO_enuSetPinDirection(DIO_u8GROUP_D,DIO_u8PIN1,DIO_u8OUTPUT);
	
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
	u8 UART_CHOICE;
	u8 Dimmer_Percentage;
	u8 Dimmer_Percentage_Tens;
	u8 Dimmer_Percentage_Ones;
	u8 ShowToUser = MAIN_MENU;
	u8 LoginSystem_u8TrialsLeft =3;
	EEPROM_ui8ReadByteFromAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,&Program_Status_Flag);
	_delay_ms(150);

	LCD_enuDisplayString("Welcome To Smart");
	LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
	LCD_enuDisplayString("Home !");
	_delay_ms (2000);
	LCD_enuClearDisplay();

	
	
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
		
	while (1)
	{
		UART_enuSendString("PRESS ENTER TO START\r\n");	
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



/************************************************************************************************************/
/************************************************************************************************************/


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
			

			u8 Local_u8Data;
			UART_enuRecieveChar(Local_u8Data);
			if(UART_enuCheck_Connection())
			{
				Program_Status_Flag = ADMIN_LOGIN_PAGE_STATUS;
			}

		}

		while (Program_Status_Flag == ADMIN_LOGIN_PAGE_STATUS)
		{
						u8 LoginSystem_u8_AdminTrueFlag;
						u8 LoginSystem_u8_Remoted_USERTrueFlag;
						LoginSystem_enuGetDataFromUserBY_UART(LoginSystem_Au8Username, LoginSystem_Au8Password);


						LoginSystem_u8_AdminTrueFlag = (LoginSystem_u8Strcmp(LoginSystem_Au8Username,LoginSystem_strAdmin.UserName)) && ( LoginSystem_u8Strcmp(LoginSystem_Au8Password,LoginSystem_strAdmin.Password) ) ;

						if(LoginSystem_u8_AdminTrueFlag == TRUE)
						{
							UART_enuSendString("\r\nLogged in Successfully");
							_delay_ms(500);
							UART_enuSendString("\r\nWelcome Admin\r\n");
							_delay_ms(500);
							LoginSystem_u8TrialsLeft =3;
							Program_Status_Flag = ADMIN_MENU_STATUS;
							
						}
						else
						{
							for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
							{
								LoginSystem_u8_Remoted_USERTrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
								if (LoginSystem_u8_Remoted_USERTrueFlag == TRUE)
								{
									if(LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == REMOTED_USER )// pro>>>>   // rem: key or not
									{
									UART_enuSendString("\r\nLogged in Successfully\r\n");
									_delay_ms(500);
									LoginSystem_u8TrialsLeft =3;
									Program_Status_Flag = REMOTED_USER_MENU_STATUS;
									break;
									}
								}
							}

						}
						if ( (LoginSystem_u8_AdminTrueFlag == FALSE)&& (LoginSystem_u8_Remoted_USERTrueFlag ==FALSE) )
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
					
			}
				
				
				/*
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

					/**************Saving the new user into the EEPROM****************
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + LoginSystem_NumOfRegisteredUsers*(2*MAX_NO_OF_LETTERS+1), LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName,MAX_NO_OF_LETTERS);
					_delay_ms(10);
					EEPROM_vWriteBlockToAddress(SAVED_USERS_INFO_ADDRESS + LoginSystem_NumOfRegisteredUsers*(2*MAX_NO_OF_LETTERS+1) + MAX_NO_OF_LETTERS, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password,MAX_NO_OF_LETTERS);
					_delay_ms(10);
					

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
		*/
		while (Program_Status_Flag == ADMIN_MENU_STATUS)
		{
			
			while(ShowToUser == MAIN_MENU)
			{
				UART_enuSendString("\r\n1-Add User");
				UART_enuSendString("\r\n2-Remove User");
				UART_enuSendString("\r\n3-Room1");
				UART_enuSendString("\r\n4-Room2");
				UART_enuSendString("\r\n5-Room3");
				UART_enuSendString("\r\n6-Room4");
				UART_enuSendString("\r\n7-Room5");
				UART_enuSendString("\r\n8-Dimmer");
				UART_enuSendString("\r\n9-Door\r\n");
			//	UART_enuSendString("\r\n10-Air Conditioner\r\n");
				
				UART_enuRecieveChar(&UART_CHOICE);
				switch(UART_CHOICE)
				{
					case '1':
					ShowToUser = ADD_USER_COMMAND;
					break;
					case '2':
					ShowToUser = REMOVE_USER_COMMAND;
					break;
					case '3':
					SPI_ui8TransmitRecive(ROOM1_LED_TOGGLE);
					ShowToUser = LOG_OUT_CHOICE;//Set the next menu to be shown to room1 menu
					break;
					case '4':
					SPI_ui8TransmitRecive(ROOM2_LED_TOGGLE);
					ShowToUser = LOG_OUT_CHOICE;//Set the next menu to be shown to room1 menu
					break;
					case '5':
					SPI_ui8TransmitRecive(ROOM3_LED_TOGGLE);
					ShowToUser = LOG_OUT_CHOICE;
					break;
					case '6':
					SPI_ui8TransmitRecive(ROOM4_LED_TOGGLE);
					ShowToUser = LOG_OUT_CHOICE;
					break;
					case '7':
					SPI_ui8TransmitRecive(ROOM5_LED_TOGGLE);
					ShowToUser = LOG_OUT_CHOICE;
					break;
					case '8':
					SPI_ui8TransmitRecive(DIMMER);
					ShowToUser = DIMMER_MENU;
					break;
					case '9':
					ShowToUser = DOOR_MENU;  
					break;
					/*case '10':
					ShowToUser = AIR_COND_MENU;
					break;
					*/
					default:
					UART_enuSendString("\r\nInvalid Choice\r\n");
					_delay_ms(1000);
					ShowToUser = MAIN_MENU;
					break;
				}
			}
			switch (ShowToUser)
			{
				case ADD_USER_COMMAND:
					if (LoginSystem_NumOfRegisteredUsers <MAX_NO_OF_USERS)
					{
						LoginSystem_enuGetDataFromUserBY_UART(LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password);
						UART_enuSendString("\r\n1-Remoted");
						UART_enuSendString("\r\n2-Promoted");
						UART_enuSendString("\r\n3-Nonremoted");
						UART_enuSendString("\r\n\r\nPriority Arrangment: Promoted > Nonremoted (LCD+Keypad) > Remoted\r\n");
						UART_enuRecieveChar(&UART_CHOICE);
						if (UART_CHOICE == '1')
						LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = REMOTED_USER;
						else if (UART_CHOICE == '2')
						LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = PROMOTED_USER;
						else if (UART_CHOICE == '3')
						LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = NORMAL_USER;
						else{
							UART_enuSendString("\r\nInvalid Choice\r\n");/////////////////////////////////////////////////////////////fel a5er
						_delay_ms(1000);}
						LoginSystem_SaveNewUser(LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers],&LoginSystem_NumOfRegisteredUsers);
						UART_enuSendString("\r\nRegistered Successfully\r\n");
					}
					else
					{
						UART_enuSendString("\r\nReached The Max Number of Stored Users");
						UART_enuSendString("\r\nCouldn't Store A New User");
						UART_enuSendString("\r\nReturning to Main Menu\r\n");
						_delay_ms(700);
					}
					ShowToUser=LOG_OUT_CHOICE;
				break;
				
				case REMOVE_USER_COMMAND:
					if (LoginSystem_RemoveUser(LoginSystem_AstrUsers,&LoginSystem_NumOfRegisteredUsers) == TRUE)
						{UART_enuSendString("\r\nRemoved Successfully\r\n");	ShowToUser=LOG_OUT_CHOICE;}
					else	{UART_enuSendString("\r\nUsername not Found \r\nReturning to Main Menu\r\n");}
					ShowToUser=MAIN_MENU;
					_delay_ms(700);
				break;
				
				case DIMMER_MENU:
					UART_enuSendString("\r\nEnter The Percentage of Brightness: ");
					UART_enuRecieveChar(&Dimmer_Percentage_Tens);
					UART_enuSendChar(Dimmer_Percentage_Tens);
					Dimmer_Percentage_Tens = Dimmer_Percentage_Tens - '0';
					
					UART_enuRecieveChar(&Dimmer_Percentage_Ones);
					UART_enuSendChar(Dimmer_Percentage_Ones);
					Dimmer_Percentage_Ones = Dimmer_Percentage_Ones -'0';
					
					Dimmer_Percentage =10*Dimmer_Percentage_Tens + Dimmer_Percentage_Ones;
					
					SPI_ui8TransmitRecive(Dimmer_Percentage);
					ShowToUser=LOG_OUT_CHOICE;
				break;
				
				case DOOR_MENU:
					UART_enuSendString("\r\nDoor Options \r\n 1-Open Door\r\n 2-Close Door\r\n 0-Return to main menu\r\n ");
					UART_enuRecieveChar(&UART_CHOICE);
					if (UART_CHOICE == '1')
					{
						SPI_ui8TransmitRecive(OPEN_DOOR_COMMAND);	
						ShowToUser = LOG_OUT_CHOICE;
					}
					else if(UART_CHOICE == '2')
					{
						SPI_ui8TransmitRecive(CLOSE_DOOR_COMMAND);
						ShowToUser = LOG_OUT_CHOICE;
					}
					else if (UART_CHOICE == '0') 
						{ShowToUser = MAIN_MENU;} 
					else{UART_enuSendString("\r\nInvalid Choice\r\n ");		ShowToUser = DOOR_MENU;}
				break;
				
				case LOG_OUT_CHOICE:
					UART_enuSendString("\r\n1-Log Out\r\n2-Return to main menu\r\n");
					UART_enuRecieveChar(&UART_CHOICE);
					if (UART_CHOICE == '1')
						{
							Program_Status_Flag = IDLE_STATUS;	
							ShowToUser = MAIN_MENU;			//That's For the next time The Program Enters the ADMIN Menu State to Start From the MAIN Menu 
							UART_enuSendString("\r\nLogged Out\r\n");
						}
				
					else if(UART_CHOICE == '2')
						{ShowToUser = MAIN_MENU;}
					else{UART_enuSendString("\r\nInvalid Choice\r\n ");		ShowToUser = LOG_OUT_CHOICE;}
				break;
				
			}
		}
		
		//if (Program_Status_Flag == USER_LOGIN_PAGE_STATUS);
		while (Program_Status_Flag == USER_LOGIN_PAGE_STATUS)///off line user should have interrupt
			{

				u8 LoginSystem_u8TrueFlag;
			//	u8 LoginSystem_u8TrialsLeft =3;

				LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_Au8Username, LoginSystem_Au8Password);
				/*********************Search The Users Array***********************/
				for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
				{
					LoginSystem_u8TrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
					if (LoginSystem_u8TrueFlag == TRUE)
					{
						LoginSystem_u8TrialsLeft =3;
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
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Welcome");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("User");
					_delay_ms(700);
					Program_Status_Flag = USER_MENU_STATUS;

			}

		}



/************************************************************************************************************/
/************************************************************************************************************/

		if (Program_Status_Flag == USER_MENU_STATUS)//off line user menu
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


