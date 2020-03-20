/* 
 * File: UART.c  
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/uart-pic-microcontroller-mplab-xc8/
*/

#include <xc.h>
#include <stdint.h>
#include "UART.h"

//******************************************************************************

//Inicialización de UART

//******************************************************************************

uint8_t UART_Init(const long int baudrate){
	SPBRG = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //Valor a meter a SPBRG, dependinte del baudrate usado
    TXSTAbits.BRGH = 1; //Indica que se usará un baudrate de alta velocidad
    TXSTAbits.SYNC = 0; //Se eligió modo asíncrono
    RCSTAbits.SPEN = 1; //Habilita los puertos seriales
    RCSTAbits.CREN = 1; // Siempre se está recibiendo datos
    TXSTAbits.TXEN = 1; // Habilita la transmisión
    TXSTAbits.TX9 = 0; // Se envian solo 8 bits
    RCSTAbits.RX9 = 0; // Se reciben solo 8 bits
}
//******************************************************************************

//Función para leer un caracter por UART

//******************************************************************************

uint8_t UART_Read(){
    if(PIR1bits.RCIF==1){//si el bit5 del registro PIR1 se ha puesto a 1, indicando que se recibio un dato
        return RCREG;//devuelve el dato almacenado en el registro RCREG, que contiene los datos recibidos por UART
    }
}
//******************************************************************************
//Función para mandar un caracter por UART
//******************************************************************************
void UART_Write(char data){
  while(!TXSTAbits.TRMT); //Mientras todavía no haya mandado el dato anterior, no envie, de lo contrario, envie
  TXREG = data;
}
/*
//******************************************************************************
//Función para leer un string por UART
//******************************************************************************
void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
}
//******************************************************************************
//Función para mandar un string por UART
//******************************************************************************

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++) //Por cada caracter del string, mandelo y aumente el contador
	  UART_Write(text[i]);
}
*/






