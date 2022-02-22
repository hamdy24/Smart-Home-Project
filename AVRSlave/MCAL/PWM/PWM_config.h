/*
 * PWM_config.h
 *
 *  Created on: Oct 30, 2021
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef MCAL_PWM_PWM_CONFIG_H_
#define MCAL_PWM_PWM_CONFIG_H_

#define TIMER_NUM 		3




typedef struct
{
	u8 TimerNum;
	u8 PWM_Type;
	u8 OC_PinConfig;
}PWM_t;

#endif /* MCAL_PWM_PWM_CONFIG_H_ */
