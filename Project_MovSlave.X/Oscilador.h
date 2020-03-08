/* 
 * File:   UART.h
 * Author: Hector A. Klée G.
 *
 * Created on 9 de febrero de 2020, 05:03 PM
 */

#ifndef __OSCILADOR_H_
#define	__OSCILADOR_H_

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#include <xc.h> // include processor files - each processor file is guarded.  

//**************************************************************
// Prototipo de la función para inicializar Oscilador Interno
// Parametros: Opción de frecuencia a utilizar ver pág. 62 Manual
//**************************************************************
void initOsc(uint8_t frec);

#endif	/*__OSCILADOR_H_*/