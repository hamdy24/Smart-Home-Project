/*
 * EEPROM_prog.c
 *
 *  Created on: Sep 30, 2021
 *      Author: Ahmed El-Gaafrawy
 */


#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum) // get bit macro used to get the value of a certain bit.

#include <avr/io.h>
#include <util/delay.h>
#include "../../MCAL/IIC/IIC_int.h"

void EEPROM_Write ( char address ,char data)

{

       TWI_Start();

       TWI_Write(0xA8); //slave address is 1010.100 and a 0 in the 8th bit to indicate Writting.

       TWI_Write(address);

       TWI_Write(data);

       TWI_Stop();

}

void EEPROM_Read (char address, char* ptr) // the function arguments are an address in the EEPROM to read from and a pointer to a memory place in the MCU to store the read data in

{

       TWI_Start();

       TWI_Write(0xA8);

       TWI_Write(address);

       TWI_Start();

       TWI_Write(0xA9);

       TWI_Read_Nack(ptr);

       TWI_Stop();




}
