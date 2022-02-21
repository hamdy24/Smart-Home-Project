#ifndef SPI_H_
#define SPI_H_


#define SPI_SCK  7
#define SPI_MISO 6
#define SPI_MOSI 5
#define SPI_SS   4


#define REGISTER_SIZE 8
#define SET_BIT(reg,bit)    reg|=(1<<bit)
#define CLR_BIT(reg,bit)    reg&=(~(1<<bit))
#define TOG_BIT(reg,bit)    reg^=(1<<bit)
#define Read_bIT(reg,bit)    (reg&(1<<bit))>>bit
#define IS_BIT_SET(reg,bit)  (reg&(1<<bit))>>bit
#define IS_BIT_CLR(reg,bit)  !((reg&(1<<bit))>>bit)
#define ROR(reg,num)         reg=(reg<<(REGISTER_SIZE-num))|(reg>>(num))
#define ROL(reg,num)        reg= (reg>>(REGISTER_SIZE-num))|(reg<<(num))



#define ROOM1_STATUS    0x11
#define ROOM2_STATUS    0x12
#define ROOM3_STATUS    0x13
#define ROOM4_STATUS    0x14
#define TV_STATUS 		0x15
#define AIR_COND_STATUS 0x16

#define ROOM1_TURN_ON    0x21
#define ROOM2_TURN_ON    0x22
#define ROOM3_TURN_ON    0x23
#define ROOM4_TURN_ON    0x24
#define TV_TURN_ON 		 0x25
#define AIR_COND_TURN_ON 0x26

#define ROOM1_TURN_OFF    0x31
#define ROOM2_TURN_OFF    0x32
#define ROOM3_TURN_OFF    0x33
#define ROOM4_TURN_OFF    0x34
#define TV_TURN_OFF 	  0x35
#define AIR_COND_TURN_OFF 0x36

#define SET_TEMPERATURE 0x40

#define DEFAULT_ACK    0xFF
#define DEMAND_RESPONSE 0xFF

#define ON_STATUS   0x01
#define OFF_STATUS  0x00


#endif /* SPI_H_ */
