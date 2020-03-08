/*
 * File:   Sensor_Interferencia.c
 * Author: Hector
 *
 * Created on 8 de marzo de 2020, 01:53 PM
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
#include "I2C.h"
#include "Oscilador.h"
//******************************************************************************
//Variables
//******************************************************************************
#define SENSOR_SIGNAL PORTDbits.RD0     //Definición de bit en Puerto D con nombre coloquial
#define SERVO_2 PORTAbits.RA0           //Definición de bit en Puerto D con nombre coloquial
uint8_t z;
uint8_t pirValue;
uint8_t sensor_signal;
uint16_t move_servo;
//******************************************************************************
// Prototipos de Funciones y Declaración de Variables
//******************************************************************************
void init(void);
//******************************************************************************
// Interrupciones
//******************************************************************************

void __interrupt() isr(void){
    //Interrupción del SSP
    if(PIR1bits.SSPIF == 1){ 

            SSPCONbits.CKP = 0;

            if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
                z = SSPBUF;                 // Read the previous value to clear the buffer
                SSPCONbits.SSPOV = 0;       // Clear the overflow flag
                SSPCONbits.WCOL = 0;        // Clear the collision bit
                SSPCONbits.CKP = 1;         // Enables SCL (Clock)
            }

            if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
                z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
                PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
                SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
                while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
                pirValue = SSPBUF;     // Guardar en el PORTD el valor del buffer de recepción
                __delay_us(250);

            }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
                z = SSPBUF;
                BF = 0;
                SSPBUF = pirValue;
                SSPCONbits.CKP = 1;
                __delay_us(250);
                while(SSPSTATbits.BF);
            }

            PIR1bits.SSPIF = 0;    
        }
}
//******************************************************************************
//Void Principal
//******************************************************************************
void main(void) {
    initOsc(7);                         // Se usa un reloj interno de 8 MHz
    init();                             //Se inicializan los puertos
        
    PORTA = 0x00;                       //Inicialización de puertos
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00; 
    while (1){
        pirValue = SENSOR_SIGNAL;
        SERVO_2 = pirValue;             //Encender o apagar el PWM dependiendo del valor del sensor
        
    }
    return;
}
//******************************************************************************
//Función de Inicialización de Puertos
//******************************************************************************
void init(void){
    TRISA = 0;                       // PORTA configurado como salida
    TRISB = 0;                       // PORTB configurado como salida
    TRISC = 0;                       // PORTC configurado como salida
    TRISD = 0b00000001;              // PORTD configurado como entrada en los bits RD0
    ANSEL = 0;                       // Pines connfigurados como entradas digitales
    ANSELH = 0;                      //Pines connfigurados como entradas digitales  
    I2C_Slave_Init(0x30);
    //INTCON = 0b11100000;                      //Habilita GIE, PIE y T0IE 
}