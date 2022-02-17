/*
 * UART_interface.h
 *
 *  Created on: Nov 9, 2021
 *      Author: hamdy
 */

#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_

ES_t UART_enuUART_Init(void);

ES_t UART_enuSendChar( u8 Copy_u8CharData);

ES_t UART_enuSendString(u8 * Copy_Au8StringData[]);

ES_t UART_enuRecieveChar(u8 * Copy_u8RxChar);

ES_t UART_enuRecieveString(u8 * Copy_Au8RxString[]);

ES_t UART_enuEnable_UART_RXEN(void);

ES_t UART_enuEnable_UART_TXEN(void);

ES_t UART_enuDisable_UART_RXEN(void);

ES_t UART_enuDisable_UART_TXEN(void);

ES_t UART_enuEnable_UART_RX_INT(void);

ES_t UART_enuDisable_UART_RX_INT(void);

u8 UART_enuCheck_Connection();

ES_t UART_enuCall_Back(volatile void (*Copy_pfun_AppFun) (void));


#endif /* MCAL_UART_UART_INTERFACE_H_ */
