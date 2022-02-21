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
#include "../LIBRARY/bit_math.h"


#include "../MCAL/DIO/DIO_int.h"

#include "../MCAL/EXTI/EXTI_config.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/SPI/SPI_int.h"
#include "../MCAL/UART/UART_interface.h"
#include "../HAL/EPROM3/EPROM_int.h"

#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"



#include "main_config.h"
#include "LoginSystem.h"

#include <util/delay.h>

u8 LoginSystem_u8AdminOnlineFlag = 0;
u8 LoginSystem_u8PromotedUserOnlineFlag = 0;
u8 ShowToUser = MAIN_MENU;
//u8 Program_Status_Flag = 0;
u8 Blocking_Flag=FALSE;
u8 LCD_KEYPAD_USER_FLAG = 0;
volatile void Button_Pressed_Interrupt()
{
	if (LoginSystem_u8AdminOnlineFlag || LoginSystem_u8PromotedUserOnlineFlag || Blocking_Flag )	//5od karar hna
	{
		LCD_KEYPAD_USER_FLAG = 0;
	}
	else
	{
		LCD_KEYPAD_USER_FLAG = 1;
		ShowToUser = MAIN_MENU;
	}
	
}
int main (void)
{
/************************************************************************************************************/
/*******************************************INITIALIZATIONS***************************************************/
	DIO_enuInit();
	SPI_vInitMaster();
	UART_enuUART_Init();
	
	LCD_enuInit();
	Keypad_enuInit();
	DIO_enuSetPinDirection(DIO_u8GROUP_D,DIO_u8PIN1,DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_C,DIO_u8PIN0,DIO_u8OUTPUT);
	EXTI_enuInit();
	__asm("sei");// GIE
	EXTI_enuCallBack((volatile void (*) (void))Button_Pressed_Interrupt);
/***************************************************************************************************/
/* ***********************************************************************************************************/
/************************************************************************************************************/


	u8 Program_Status_Flag = 0;
	User_t LoginSystem_strAdmin;
	User_t LoginSystem_AstrUsers[MAX_NO_OF_USERS];
	u8 LoginSystem_NumOfRegisteredUsers=0;
	u8 LoginSystem_Au8Username[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
	u8 LoginSystem_Au8Password[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];

	u8 Keypad_Pressed_Key;
	u8 On_Devices=0;
	u8 UART_CHOICE;
	u8 Dimmer_Percentage;
	u8 Dimmer_Percentage_Tens;
	u8 Dimmer_Percentage_Ones;
//	u8 ShowToUser = MAIN_MENU;
	u8 LoginSystem_u8TrialsLeft =3;
	u8 Current_State=0;
	
	LCD_enuDisplayString("Welcome To Smart");
	LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
	LCD_enuDisplayString("Home !");
	EEPROM_ui8ReadByteFromAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,&Program_Status_Flag);
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
				_delay_ms(150);
				EEPROM_vWriteBlockToAddress(SAVED_ADMIN_INFO_ADDRESS + MAX_NO_OF_LETTERS ,LoginSystem_strAdmin.Password , MAX_NO_OF_LETTERS);
				_delay_ms(150);

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
				_delay_ms(150);
				

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

		if (Program_Status_Flag == IDLE_STATUS)
		{
			u8 Local_u8Iterator = 1;
			if ((Current_State == -0))
			{LCD_enuClearDisplay();
				LCD_enuDisplayString("All Devices");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("Are Off");
			}
			while ( Program_Status_Flag == IDLE_STATUS)
			{	
				On_Devices = ( GET_BIT( Current_State, Local_u8Iterator) )*Local_u8Iterator;
				switch (On_Devices)
				{
					
					case State_ROOM_1:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Room1 On");
					_delay_ms(1000);
					break;
					case State_ROOM_2:
					if (!GET_BIT( Current_State, State_ROOM_1))
					{
						LCD_enuClearDisplay();
					}
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Room2 On");
					_delay_ms(1000);
					break;
					case State_ROOM_3:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Room3 On");
					_delay_ms(1000);
					break;
					case State_ROOM_4:
					if (!GET_BIT( Current_State, State_ROOM_3))
					{
						LCD_enuClearDisplay();
					}
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Room4 On");
					_delay_ms(1000);
					break;
				/*	case State_ROOM_5:
					LCD_enuDisplayString("Room 5 controlled");
					break;
					case State_Air_Cond:
					LCD_enuDisplayString("Air Cond controlled");
					break; */
					case State_Door:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Door is Opened");
					_delay_ms(1000);
					break;
					case State_Dimmer:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Dimmer: ");
					LCD_enuDisplayUnsignedInteger(Dimmer_Percentage);
					_delay_ms(1000);
					break;
				}
					Local_u8Iterator++;
					if (Local_u8Iterator == 7)
						Local_u8Iterator =1;
					if (LCD_KEYPAD_USER_FLAG == 1)
					{
						Program_Status_Flag = USER_LOGIN_PAGE_STATUS;
					}
					else if(UART_enuCheck_Connection())
					{
						Program_Status_Flag = REMOTED_SYSTEM_LOGIN_PAGE_STATUS;
					}
			}
			
				
		
		//	while (Program_Status_Flag == IDLE_STATUS)
			/*if (LCD_KEYPAD_USER_FLAG == 1)
			{
				Program_Status_Flag = USER_LOGIN_PAGE_STATUS;
			}
			else if(UART_enuCheck_Connection())
			{
				Program_Status_Flag = REMOTED_SYSTEM_LOGIN_PAGE_STATUS;
			}*/
		}
	
	
		while (Program_Status_Flag == REMOTED_SYSTEM_LOGIN_PAGE_STATUS)
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
				LoginSystem_u8AdminOnlineFlag = TRUE;
				LoginSystem_u8TrialsLeft =3;
				Program_Status_Flag = REMOTED_SYSTEM_MENU_STATUS;	
			}
			else
			{
				for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
				{
								
					LoginSystem_u8_Remoted_USERTrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
					if ( (LoginSystem_u8_Remoted_USERTrueFlag == TRUE)  )
					{
						if ( (LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == REMOTED_USER) || (LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == PROMOTED_USER) )
						{
							UART_enuSendString("\r\nLogged in Successfully\r\n");
							_delay_ms(500);
							LoginSystem_u8TrialsLeft =3;
							Program_Status_Flag = REMOTED_SYSTEM_MENU_STATUS;
							if (LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == PROMOTED_USER)
							{
								LoginSystem_u8PromotedUserOnlineFlag = TRUE;
							}
							break;
						}
						else
						{	
							LoginSystem_u8_Remoted_USERTrueFlag = FALSE;
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
							//_delay_ms(1000);
				if (LoginSystem_u8TrialsLeft == 0)
				{
					Program_Status_Flag = BLOCKING_STATUS;
				}
			}
					
		}
				
		
		while (Program_Status_Flag == REMOTED_SYSTEM_MENU_STATUS)
		{
			
			while(ShowToUser == MAIN_MENU)
			{
				//The first five chioces will Appear for Admin or Remoted user 
				UART_enuSendString("\r\n1-Room1");
				UART_enuSendString("\r\n2-Room2");
				UART_enuSendString("\r\n3-Room3");
				UART_enuSendString("\r\n4-Room4");
			//	UART_enuSendString("\r\n5-Room5");
			//	UART_enuSendString("\r\n10-Air Conditioner\r\n");
				UART_enuSendString("\r\n5-Dimmer");
				if (LoginSystem_u8AdminOnlineFlag ==TRUE) //The Last 3 options will Appear only to Admin
				{
					UART_enuSendString("\r\n6-Door");
					UART_enuSendString("\r\n7-Add User");
					UART_enuSendString("\r\n8-Remove User\r\n");
					//UART_enuSendString("\r\n9-Promote User");
					UART_enuRecieveChar(&UART_CHOICE);
				}
				else
				{
					UART_enuSendString("\r\n");		//For the shape of menu
					UART_enuRecieveChar(&UART_CHOICE);
					if (UART_CHOICE > '5')
						UART_CHOICE =100;		//If the program enter this else, then a Remoted User is in control. So, We want him to go to default in the switch case if he entered by mistake a number that corresponds An admin option in the switch case
				}
				
				//UART_enuRecieveChar(&UART_CHOICE);
				switch(UART_CHOICE)
				{
					case '1':
						SPI_ui8TransmitRecive(ROOM1_LED_TOGGLE);
						ToggleBit(&Current_State,1);
						ShowToUser = LOG_OUT_CHOICE;//Set the next menu to be shown to room1 menu
					break;
					
					case '2':
						SPI_ui8TransmitRecive(ROOM2_LED_TOGGLE);
						ToggleBit(&Current_State,2);
						ShowToUser = LOG_OUT_CHOICE;//Set the next menu to be shown to room1 menu
					break;
					
					case '3':
						SPI_ui8TransmitRecive(ROOM3_LED_TOGGLE);
						ToggleBit(&Current_State,3);
						ShowToUser = LOG_OUT_CHOICE;
					break;
					
					case '4':
						SPI_ui8TransmitRecive(ROOM4_LED_TOGGLE);
						ToggleBit(&Current_State,4);
						ShowToUser = LOG_OUT_CHOICE;
					break;
					
					/*case '5':
						SPI_ui8TransmitRecive(ROOM5_LED_TOGGLE);
						ShowToUser = LOG_OUT_CHOICE;
					break;*/
					
					case '5':
						SPI_ui8TransmitRecive(DIMMER);
						SetBit(&Current_State,5);
						ShowToUser = DIMMER_MENU;
					break;
					
					case '6':
						ShowToUser = DOOR_MENU;  
					break;
					
					case '7':
						if (LoginSystem_NumOfRegisteredUsers <MAX_NO_OF_USERS)
							{
								LoginSystem_enuGetDataFromUserBY_UART(LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].UserName, LoginSystem_AstrUsers[LoginSystem_NumOfRegisteredUsers].Password);
								ShowToUser = PRIORITIES_MENU;
							}
						else
							{
								UART_enuSendString("\r\nReached The Max Number of Stored Users");
								UART_enuSendString("\r\nCouldn't Store A New User");
								UART_enuSendString("\r\nReturning to Main Menu\r\n");
								ShowToUser = MAIN_MENU;
								_delay_ms(700);
							}
					break;
					case '8':
						if (LoginSystem_RemoveUser(LoginSystem_AstrUsers,&LoginSystem_NumOfRegisteredUsers) == TRUE)
						{		UART_enuSendString("\r\nRemoved Successfully\r\n");
								ShowToUser=LOG_OUT_CHOICE;
						}
						else	{UART_enuSendString("\r\nUsername not Found \r\nReturning to Main Menu\r\n");}
						ShowToUser=MAIN_MENU;
						_delay_ms(700);
					break;
					/*case '9':
						ShowToUser = PROMOTE_USER_COMMAND;		//na5od karar
					break;
					case '10':
						ShowToUser = AIR_COND_MENU;			//na5od karar
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
				case PRIORITIES_MENU:
						UART_enuSendString("\r\n1-Remoted");
						UART_enuSendString("\r\n2-Promoted");
						UART_enuSendString("\r\n3-Nonremoted");
						UART_enuSendString("\r\n\r\nPriority Arrangment: Promoted > Nonremoted (LCD+Keypad) > Remoted\r\n");
						UART_enuRecieveChar(&UART_CHOICE);
						if (UART_CHOICE == '1')
							{LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = REMOTED_USER;
							LoginSystem_SaveNewUser(LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers],&LoginSystem_NumOfRegisteredUsers);
							ShowToUser = LOG_OUT_CHOICE;
							UART_enuSendString("\r\nRegistered Successfully\r\n");}
						else if (UART_CHOICE == '2')
							{LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = PROMOTED_USER;
							LoginSystem_SaveNewUser(LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers],&LoginSystem_NumOfRegisteredUsers);
							ShowToUser = LOG_OUT_CHOICE;
							UART_enuSendString("\r\nRegistered Successfully\r\n");}
						else if (UART_CHOICE == '3')
							{LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers].User_Priority = LCD_KEYPAD_USER;
							LoginSystem_SaveNewUser(LoginSystem_AstrUsers [LoginSystem_NumOfRegisteredUsers],&LoginSystem_NumOfRegisteredUsers);
							ShowToUser = LOG_OUT_CHOICE;
							UART_enuSendString("\r\nRegistered Successfully\r\n");}
						else
						{
							UART_enuSendString("\r\nInvalid Choice\r\n");
							_delay_ms(1000);	
						}
						
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
						SetBit(&Current_State,6);	
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
							if (LoginSystem_u8AdminOnlineFlag == TRUE)	//If an Admin was online and wants to log out
								LoginSystem_u8AdminOnlineFlag = FALSE;	
							else if (LoginSystem_u8PromotedUserOnlineFlag == TRUE)	//If a promoted user was online and wants to log out
								LoginSystem_u8PromotedUserOnlineFlag = FALSE;
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
				LCD_enuClearDisplay();
				LCD_enuDisplayString("User Login");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("Attempt Detected");
				_delay_ms(700);
				u8 LoginSystem_u8TrueFlag;
				LoginSystem_enuGetDataFromUserByKeypad(LoginSystem_Au8Username, LoginSystem_Au8Password);
				/*********************Search The Users Array***********************/
				for (u8 Local_u8Iterator=0; Local_u8Iterator < LoginSystem_NumOfRegisteredUsers; Local_u8Iterator++)
				{
					LoginSystem_u8TrueFlag = LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].UserName,LoginSystem_Au8Username) && LoginSystem_u8Strcmp (LoginSystem_AstrUsers[Local_u8Iterator].Password,LoginSystem_Au8Password);
					if ( LoginSystem_u8TrueFlag == TRUE  )
					{	
						if ( (LoginSystem_AstrUsers[Local_u8Iterator].User_Priority == LCD_KEYPAD_USER) )
						{
							LoginSystem_u8TrialsLeft =3;
							LCD_enuClearDisplay();
							LCD_enuDisplayString("Logged in");
							LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
							LCD_enuDisplayString("Successfully");
							_delay_ms(700);
							LCD_enuClearDisplay();
							LCD_enuDisplayString("Welcome User");
							_delay_ms(700);
							Program_Status_Flag = USER_MENU_STATUS;
							break;
						}
						else
						{
							LoginSystem_u8TrueFlag = FALSE;
							break;
						}
					}
					
				}
				if (LoginSystem_u8TrueFlag == FALSE)
				{
					LoginSystem_u8TrialsLeft--;
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Wrong Username");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("or Password");
					_delay_ms(700);
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Remaining Trials: ");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayUnsignedInteger(LoginSystem_u8TrialsLeft);
					_delay_ms(700);
					if (LoginSystem_u8TrialsLeft == 0)
					{
						Program_Status_Flag = BLOCKING_STATUS;
					}
				}

		}
/************************************************************************************************************/
/************************************************************************************************************/

		while (Program_Status_Flag == USER_MENU_STATUS)//off line user menu
		{
			while(ShowToUser == MAIN_MENU)
			{
				LCD_enuClearDisplay();			
				LCD_enuDisplayString("1:Room1 2:Room2");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("3:Room3 4:More");
				Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
				switch (Keypad_Pressed_Key)
				{
				case '1':
					SPI_ui8TransmitRecive(ROOM1_LED_TOGGLE);
					ToggleBit(&Current_State,1);
					ShowToUser = LOG_OUT_CHOICE;
				break;
				
				case '2':
					SPI_ui8TransmitRecive(ROOM2_LED_TOGGLE);
					ToggleBit(&Current_State,2);
					ShowToUser = LOG_OUT_CHOICE;
				break;
				
				case '3':
					SPI_ui8TransmitRecive(ROOM3_LED_TOGGLE);
					ToggleBit(&Current_State,3);
					ShowToUser = LOG_OUT_CHOICE;
				break;
				
				case '4':
					ShowToUser = MORE_MENU;
				break;
				
				default:
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Invalid Choice");
				_delay_ms(700);
				break;
				}
			}
			
			while (ShowToUser == MORE_MENU)
			{
				LCD_enuClearDisplay();
				LCD_enuDisplayString("1:Room4 2:Dimmer");
				LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
				LCD_enuDisplayString("0:Return");
				Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
				switch (Keypad_Pressed_Key)
				{
					case '1':
					SPI_ui8TransmitRecive(ROOM4_LED_TOGGLE);
					ToggleBit(&Current_State,4);
					ShowToUser = LOG_OUT_CHOICE;
					break;
					
					case '2':
					SPI_ui8TransmitRecive(DIMMER);
					
					ShowToUser = DIMMER_MENU;
					break;
					
					case '0':
					ShowToUser = MAIN_MENU;
					break;
					
					default:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Invalid Choice");
					_delay_ms(700);
					break;
				}
			}
			switch(ShowToUser)
			{
				case DIMMER_MENU:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Brightness");
					LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
					LCD_enuDisplayString("Percentage:");
					Keypad_enuGetPressedKey(&Dimmer_Percentage_Tens);
					LCD_enuSendData(Dimmer_Percentage_Tens);
					Dimmer_Percentage_Tens = Dimmer_Percentage_Tens - '0';
					Keypad_enuGetPressedKey(&Dimmer_Percentage_Ones);
					LCD_enuSendData(Dimmer_Percentage_Ones);
					Dimmer_Percentage_Ones = Dimmer_Percentage_Ones -'0';
					Dimmer_Percentage =10*Dimmer_Percentage_Tens + Dimmer_Percentage_Ones;
					SPI_ui8TransmitRecive(Dimmer_Percentage);
					_delay_ms(700);
					SetBit(&Current_State,5);
					ShowToUser=LOG_OUT_CHOICE;
				break;
				
				case LOG_OUT_CHOICE:
					LCD_enuClearDisplay();
					LCD_enuDisplayString("1:Log Out 2:Ret");
					Keypad_enuGetPressedKey(&Keypad_Pressed_Key);
					if (Keypad_Pressed_Key == '1')
					{
						Program_Status_Flag = IDLE_STATUS;
						ShowToUser = MAIN_MENU;			//That's For the next time The Program Enters the ADMIN Menu State to Start From the MAIN Menu
						LCD_KEYPAD_USER_FLAG = 0;
						LCD_enuClearDisplay();
						LCD_enuDisplayString("Logged Out");
						
						_delay_ms(700);
					}
					else if(Keypad_Pressed_Key == '2')
						{ShowToUser = MAIN_MENU;}
					else{LCD_enuClearDisplay();		LCD_enuDisplayString("Invalid Choice");	 _delay_ms(700);	ShowToUser = LOG_OUT_CHOICE;}
				break;
			}
		}//end of the LCD_KEYPAD_MENU_STATUS

	/************************************************************************************************************/
	/************************************************************************************************************/
		if(Program_Status_Flag == BLOCKING_STATUS)
		{
			EEPROM_vWriteByteToAddress(SAVED_INITIAL_PROGRAM_STATUS_ADDRESS,BLOCKING_STATUS);
			Blocking_Flag=TRUE;
			LCD_enuClearDisplay();
			LCD_enuDisplayString("You are Blocked");
			LCD_enuSetCursorPosition(LCD_u8YDIM_1,LCD_u8XDIM_0);
			LCD_enuDisplayString("From The System");
			DIO_enuSetPinValue(DIO_u8GROUP_C,DIO_u8PIN0,DIO_u8HIGH);
			while (Program_Status_Flag == BLOCKING_STATUS);
			
		}//end of the Blocking Status
		

	}//end of the while (1)



	return 0;
}


