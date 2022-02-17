/*
 * IIC_int.h
 *
 *  Created on: Feb 17, 2022
 *      Author: hamdy
 */

#ifndef MCAL_IIC_IIC_INT_H_
#define MCAL_IIC_IIC_INT_H_

void TWI_Init (void);

void TWI_Start (void);

void TWI_Stop (void);

void TWI_Write (char data);

void TWI_Read_Nack (char* ptr);


#endif /* MCAL_IIC_IIC_INT_H_ */
