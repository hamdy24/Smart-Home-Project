/*
 * SPI_int.h
 *
 *  Created on: Feb 18, 2022
 *      Author: hamdy
 */

#ifndef MCAL_SPI_SPI_INT_H_
#define MCAL_SPI_SPI_INT_H_

void SPI_vInitMaster (void);

void SPI_vInitSlave (void);

u8 SPI_ui8TransmitRecive (u8 data);

#endif /* MCAL_SPI_SPI_INT_H_ */
