/*
 * EEPROM_int.h
 *
 *  Created on: Sep 30, 2021
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef EEPROM_INT_H_
#define EEPROM_INT_H_


void EEPROM_Write ( char address ,char data);

void EEPROM_Read (char address, char* ptr);

#endif /* EEPROM_INT_H_ */
