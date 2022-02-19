/*
 * LoginSystem.h
 *
 *  Created on: 15 Feb 2022
 *      Author: Dell
 */

#ifndef APP_LOGINSYSTEM_H_
#define APP_LOGINSYSTEM_H_



ES_t LoginSystem_enuGetDataFromUserByKeypad(u8 *Copy_Au8Username, u8 *Copy_Au8Password);
ES_t LoginSystem_enuGetDataFromUserBY_UART(u8 *Copy_Au8Username, u8 *Copy_Au8Password);
ES_t LoginSystem_SaveNewUser(User_t Copy_strNewUser,u8 *Copy_u8NumofRegisteredUsers);
u8 LoginSystem_u8Strcmp (char *Copy_Au8Sring1, char *Copy_Au8Sring2);

void CLOSE_LEDS();
void OPEN_LEDS(u8 freq,u8 state);
void ServoMotor_Door(u8 Copy_u8Angle);


#endif /* APP_LOGINSYSTEM_H_ */
