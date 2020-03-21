/*
 * File:   Master_Lab_4.c
 * Author: Hector
 *
 * Created on 16 de febrero de 2020, 08:11 AM
 */
//******************************************************************************
// Palabra de Configuración
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Definición de librerías
//******************************************************************************
#define _XTAL_FREQ 8000000 //Se define la frecuencia del oscilador para el delay
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

#include "Oscilador.h"
#include "UART.h"
#include "SPI.h"
//******************************************************************************
// Variables
//******************************************************************************
uint8_t Sens_INT;
uint8_t Sens_MOV;
uint8_t Sens_VIB;
uint8_t Sens_TEMP;
uint8_t Sens_PESO;
char RASPBERRY;
char S0;
//******************************************************************************
// Funciones prototipo
//******************************************************************************
void init(void);
//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    //Interrupción del SSP
           
    if(PIR1bits.SSPIF==1){
        RASPBERRY = spiRead();
        
        if(RASPBERRY == 0){
            spiWrite(Sens_INT);
            SSPSTATbits.BF= 0;
            PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 1){
            spiWrite(Sens_MOV);
            SSPSTATbits.BF= 0;
            PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 2){
            spiWrite(Sens_VIB);
            SSPSTATbits.BF= 0;
            PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 3){
            spiWrite(Sens_TEMP);
            SSPSTATbits.BF= 0;
            PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 4){
            spiWrite(Sens_PESO);
            SSPSTATbits.BF= 0;
            PIR1bits.SSPIF = 0;
          return;
        }
       
          SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
        return;
    }
    if(PIR1bits.RCIF == 1){
        if(S0 == 0){
        Sens_INT= UART_Read();
        PIR1bits.RCIF = 0;
        S0=1;
        return;
        }
         if(S0 == 1){
        Sens_MOV= UART_Read();
        PIR1bits.RCIF = 0;
        S0=2;
        return;
        }
         if(S0 == 2){
        Sens_VIB = UART_Read();
        PIR1bits.RCIF = 0;
        S0=3;
        return;
        }
         if(S0 == 3){
        Sens_TEMP = UART_Read();
        PIR1bits.RCIF = 0;
        S0=4;
        return;
        }
         if(S0 == 4){
        Sens_PESO= UART_Read();
        PIR1bits.RCIF = 0;
        S0 = 0;
        return;
        }
    }
    PIR1bits.RCIF = 0;
    return;
   
}
//******************************************************************************
//Void Principal
//******************************************************************************
void main(void) {
    initOsc(7);             //Se usa un reloj interno de 8 MHz
    init();                 //Se inicializan los puertos
    UART_Init (9600);
    
    while(1){
        PORTB = RASPBERRY;
        UART_Write(Sens_INT);
        __delay_ms(1);
        UART_Write(Sens_MOV);
        __delay_ms(1);
        UART_Write(Sens_VIB);
        __delay_ms(1);
        UART_Write(Sens_TEMP);
        __delay_ms(1);
        UART_Write(Sens_PESO);
        __delay_ms(1);
    }
    return;
}

void init (void){
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b11010000;
    TRISD = 0;
    TRISDbits.TRISD1 = 0;
    ANSEL = 0;
    ANSELH = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE1bits.SSPIE = 1;
    RASPBERRY = 0;
    S0 = 0;
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}