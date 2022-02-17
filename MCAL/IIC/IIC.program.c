/*
 * IIC.program.c
 *
 *  Created on: Feb 17, 2022
 *      Author: hamdy
 */
#define F_CPU 16000000ul
#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum) // get bit macro used to get the value of a certain bit.
#include <avr/io.h>
#include <util/delay.h>

void TWI_Init (void)

{

       //set_bit(TWCR,6);

       TWSR=0;

       TWBR=0x07;

       TWCR|=(1<<TWEN);

}




void TWI_Start (void)

{

       TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

       while (get_bit(TWCR,TWINT)==0)

       {

       }

}




void TWI_Stop (void)

{

       TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);

}




void TWI_Write (char data)

{

       TWDR=data;

       TWCR= (1<<TWINT)|(1<<TWEN);

       while (get_bit(TWCR,TWINT)==0)

       {

       }

}




void TWI_Read_Nack (char* ptr) // The function argument is a pointer to a memory place in the MCU to store the received data in

{

       TWCR=(1<<TWINT)|(1<<TWEN);

       while (get_bit(TWCR,TWINT)==0)

       {

       }

       *ptr=TWDR;

}



