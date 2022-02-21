#include <avr/io.h>
#include "SPI.h"
#include "../../LIBRARIES/stdTypes.h"

void SPI_vInitMaster (void)
{
	DDRB |=(1<<SPI_MOSI) |(1<<SPI_SS)|(1<<SPI_SCK);//Set the port B mosi(master out slave in ) ,port B ss(slave select),sck(spi clock)  pins as output
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);//Enable the SPI,master mode ,Set the clock speed to F/16
}

void SPI_vInitSlave (void)
{
	DDRB |= (1<<SPI_MISO) ;//Set the port B miso(master in slave out ) pin as output
	SPCR |= (1<<SPE);//Enable the SPI
}

u8 SPI_ui8TransmitRecive (u8 data)
{
	SPDR = data;//move the given data to SPI Data register
	
	//in case of master send data and wait till the data are fully transmitted
	//in case of slave wait the data to be transmitted by the master
	while (((SPSR&(1<<SPIF))>>SPIF)==0)	//wait till the transmitting is done
	;									//SPI flag is set to one if the transmission is done
	
	return SPDR;//return the value of SPI Data register after the transmission or receiving is done
}
