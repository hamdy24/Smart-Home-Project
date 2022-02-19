/*
 * main_config.h
 *
 *  Created on: 11 Feb 2022
 *      Author: Dell
 */

#ifndef APP_MAIN_CONFIG_H_
#define APP_MAIN_CONFIG_H_

#define TRUE									 1
#define FALSE									 0
#define NULL_CHARACTER 							'\0'
#define NULL_CHARACTER_COUNT					 1

#define NORMAL_USER	   						 	 0  // Keypad
#define ADMIN_PRIORITY 					   		 1	//admin
#define REMOTED_USER						   	 2	//dont have exceptin
#define PROMOTED_USER						 	 3 	//have exception above keypad user

#define MAX_NO_OF_USERS							 4
#define MAX_NO_OF_LETTERS						 4
/**********************************EEPROM Used Addresses***********************************/
#define SAVED_INITIAL_PROGRAM_STATUS_ADDRESS	 0x00
#define	SAVED_ADMIN_INFO_ADDRESS				 0X01
#define NO_OF_REGISTERED_USERS_ADDRESS 			 0x11
#define	SAVED_USERS_INFO_ADDRESS				 0x12
/**********************************Program Status Flag Modes***********************************/
#define FIRST_TIME_USE_STATUS 					 0xff
#define LOAD_LOGIN_SYSTEM_DATABASE				 0x00
#define MAIN_MENU_STATUS 0x01
#define ADMIN_LOGIN_PAGE_STATUS 				 0X02
#define USER_LOGIN_PAGE_STATUS					 0X03
#define ADMIN_MENU_STATUS 						 0x04
#define REMOTED_USER_MENU_STATUS			 	 0x08
#define USER_MENU_STATUS 						 0x05
#define BLOCKING_STATUS 						 0x06
#define IDLE_STATUS 							 0x07


#define State_ROOM_1							 1
#define State_ROOM_2							 2
#define State_ROOM_3							 3
#define State_ROOM_4							 4
#define State_ROOM_5							 5
#define State_Dimmer							 6
#define State_Air_Cond							 7
#define State_Door								 8

typedef struct {
	u8 UserName[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
	u8 Password[MAX_NO_OF_LETTERS+NULL_CHARACTER_COUNT];
	u8 	User_Priority;
}User_t;


#endif /* APP_MAIN_CONFIG_H_ */
