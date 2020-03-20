/* 
 * File:   UART.h
 * Author: Hector A. Klée G.
 *
 * Created on 9 de febrero de 2020, 05:03 PM
 */

#ifndef __UART_H_
#define	__UART_H_

//******************************************************************************

//Se define para poder utilizar la frecuencia de oscilación en el cálculo del Baud Rate

//******************************************************************************
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

//******************************************************************************

//Prototipos de Funciones

//******************************************************************************
uint8_t UART_Init(const long int baudrate);
uint8_t UART_Read(void);
void UART_Read_Text(char *Output, unsigned int length);
void UART_Write(char data);
void UART_Write_Text(char *text);

#endif	/* UART_H */

