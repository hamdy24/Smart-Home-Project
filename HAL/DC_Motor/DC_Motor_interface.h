/*
 * DC_Motor_interface.h
 *
 *  Created on: Feb 17, 2022
 *      Author: hamdy
 */

#ifndef HAL_DC_MOTOR_DC_MOTOR_INTERFACE_H_
#define HAL_DC_MOTOR_DC_MOTOR_INTERFACE_H_

ES_t DC_Motor_enuInit(u8 Copy_u8Motor1_State , u8 Copy_u8Motor2_State);

ES_t DC_Motor_enuStart(u8 Copy_u8MotorNum , u8 Copy_u8Motor_Direction);

ES_t DC_Motor_enuStop(u8 Copy_u8MotorNum);

#endif /* HAL_DC_MOTOR_DC_MOTOR_INTERFACE_H_ */
