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

#include "Oscilador.h"
#include "UART.h"
#include "SPI.h"

//******************************************************************************
// Funciones prototipo
//******************************************************************************
void init(void);
//******************************************************************************
// Variables
//******************************************************************************
uint8_t Sens_INT;
uint8_t Sens_MOV;
uint8_t Sens_VIB;
uint8_t Sens_TEMP;
uint8_t Sens_PESO;
//******************************************************************************
//Void Principal
//******************************************************************************
void main(void) {
    initOsc(7);             //Se usa un reloj interno de 8 MHz
    init();                 //Se inicializan los puertos
    UART_Init (9600);
    
    while(1){
        //-------------------SENSOR DE INTERRFERENCIA----------------------
        __delay_ms(100);
        Sens_INT = UART_Read();          //SE RECIVE EL VALOR POR UART
        spiWrite(Sens_INT);              //SE ENVIA POR SPI 
        __delay_ms(200);
        //-----------------SENSOR MOVIMIENTO------------------------------
        Sens_MOV = UART_Read();         // SE RECIVE EL VALOR POR UART
        spiWrite(Sens_MOV);             //SE ENVIA POR SPI        
        __delay_ms(200);
        //----------------SENSOR DE VIBRACION------------------------------
        Sens_VIB = UART_Read;           //SE RECIVE EL VALOR POR UART
        spiWrite(Sens_VIB);             //SE ENVIA POR SPI
        __delay_ms (200);
        //------------------SENSOR DE TEMPERATURA--------------------------
        Sens_TEMP = UART_Read();      //SE RECIVE EL VALOR POR UART
        spiWrite(Sens_TEMP);          //SE ENVIA POR SPI
        __delay_ms(200);
        //------------------SENSOR DE PESO--------------------------------
        Sens_PESO = UART_Read();        //SE RECIVE EL VALOR POR UART
        spiWrite(Sens_PESO);            //SE ENVIA POR SPI
        __delay_ms(100);
        
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
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}