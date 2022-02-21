/*
 * slave.c
 *
 *  Created on: Feb 19, 2022
 *      Author: hamdy
 */

// #includes     // variable decleration
// states of rooms door air  dimmer
#include "../LIBRARIES/stdTypes.h"
#include "../LIBRARIES/errorStates.h"
#include "../LIBRARIES/AVR_REG.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/ADC/ADC_int.h"
#include "../MCAL/SPI/SPI_int.h"
#include "../MCAL/TIMER1/Timer1_int.h"

#include "../HAL/DC_Motor/DC_Motor_interface.h"
#include "../HAL/LM35/LM35_int.h"

#include "main_config.h"
#include "LoginSystem.h"
#include <util/delay.h>


int main()
{
/*****************************************************************************************************/
/*********************************************INITIALIZATION******************************************/
/*****************************************************************************************************/
 	    SPI_vInitSlave ();
		Timer1_enuInit();
		LM35_enuInit();
		DC_Motor_enuInit(1,0);
/*****************************************************************************************************/
/**********************************************DECLARATIONS*******************************************/
/*****************************************************************************************************/
		u8 Master_Command;


/*****************************************************************************************************/
/*****************************************DEVICES OUTPUT CONFIGURATION*********************************************************/
/*****************************************************************************************************/
		DIO_enuSetPinDirection(DIO_u8GROUP_D,DIO_u8PIN7,DIO_u8OUTPUT);/// output servo
			__asm("sei");// GIE



	while (1)
	{

/************************************************************************************************************/
/******************************AIR CONDITIONER AUTOMATIC CONTROL*********************************************/
/************************************************************************************************************/

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



/***************************************************************************************************************/
/***************************************************************************************************************/

		Master_Command = SPI_ui8TransmitRecive(255);///// ack argument misssed
		switch(Master_Command)
		{
		case ROOM1_LED_TOGGLE:
			DIO_enuSetPinDirection(DIO_u8GROUP_A,DIO_u8PIN1,DIO_u8OUTPUT);
			DIO_enuTogPinValue(DIO_u8GROUP_A,DIO_u8PIN1);
			break;
		case ROOM2_LED_TOGGLE:
			DIO_enuSetPinDirection(DIO_u8GROUP_A,DIO_u8PIN2,DIO_u8OUTPUT);
			DIO_enuTogPinValue(DIO_u8GROUP_A,DIO_u8PIN2);
			break;
		case ROOM3_LED_TOGGLE:
			DIO_enuSetPinDirection(DIO_u8GROUP_A,DIO_u8PIN3,DIO_u8OUTPUT);
			DIO_enuTogPinValue(DIO_u8GROUP_A,DIO_u8PIN3);
			break;
		case ROOM4_LED_TOGGLE:
		 	DIO_enuSetPinDirection(DIO_u8GROUP_A,DIO_u8PIN4,DIO_u8OUTPUT);
			DIO_enuTogPinValue(DIO_u8GROUP_A,DIO_u8PIN4);
			break;
		case ROOM5_LED_TOGGLE:
			DIO_enuSetPinDirection(DIO_u8GROUP_A,DIO_u8PIN5,DIO_u8OUTPUT);
			DIO_enuTogPinValue(DIO_u8GROUP_A,DIO_u8PIN5);
			break;
		case DIMMER: //there is onther dimmmer spi opt >> percentage

			break;
		case OPEN_DOOR_COMMAND:
			ServoMotor_Door(90);
			break;
		case CLOSE_DOOR_COMMAND:
			ServoMotor_Door(0);
			break;
		case OPEN_AirCond_COMMAND:
			DC_Motor_enuStart(1,1);
			break;
		case CLOSE_AirCond_COMMAND:
			DC_Motor_enuStop(1);
			break;
		default:
			break;
		}


	}
}
