/*
 * File:   MASTER_Proyecto.c
 * Author: Hector
 *
 * Created on 6 de marzo de 2020, 12:10 PM
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
#include <math.h>
#include "I2C.h"
#include "LCD.h"
#include "Oscilador.h"
#include "UART.h"

//******************************************************************************
// Funciones prototipo
//******************************************************************************
void init(void);
uint8_t Val_STR(uint8_t num);
//******************************************************************************
// Variables
//******************************************************************************
uint8_t Val_INT = 0;
uint8_t Val_MOV = 0;
uint8_t Val_VIB = 0;
uint8_t Val_TEMP = 0;
uint8_t Val_PESO = 0;

float ADC_TEMP_V = 0;
float ADC_PESO_V;
float POT_cien = 0;
float RES_cien = 0;

const float invBeta = 1.00/3380.00;
const float adcMax = 1023.00;
const float invT0 = 1.00/298.15;
float K;
float C;

uint8_t TEMP_EN = 0;
uint8_t TEMP_EN_1 = 0;

uint8_t POT_EN = 0;
uint8_t RES_EN = 0;
uint8_t POT_D1 = 0;
uint8_t POT_D2 = 0;
uint8_t RES_D1 = 0;
uint8_t RES_D2 = 0;
uint8_t CONT_U = 0;
uint8_t CONT_D = 0;
uint8_t CONT_C = 0;
uint8_t i=0;

uint16_t DECI_1_POT = 0;
uint16_t DECI_2_POT = 0;
uint16_t DECI_1_RES = 0;
uint16_t DECI_2_RES = 0;
//******************************************************************************
//Void Principal
//******************************************************************************
void main(void) {
    initOsc(7); // Se usa un reloj interno de 8 MHz
    init(); //Se inicializan los puertos
    lcd_init(); //Se inicializa la LCD
    UART_Init(9600);
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0; //Se inicializan todas los puertos en 0
    
    while (1){
       
        LCD_POINT(1,2);
        lcd_msg("INT"); //Se envía el string INT para indicar que los datos mostrados son SENSOR DE INTERFERENCIA
        LCD_POINT(1,6);
        lcd_msg("MOV"); //Se envía el string MOV para indicar que los datos mostrados son del SENSOR DE MOVIMIENTO
        LCD_POINT(1,10);
        lcd_msg("VIB"); //Se envía el string VIB para indicar que los datos mostrados son del SENSOR DE VIBRACION
        
        //**********************************************************************
        // LECTURA DE DATOS POR I2C
        //**********************************************************************
        //Lectura de Sensor de Interferencia
        //**********************************************************************
        I2C_Master_Start();
        I2C_Master_Write(0x31);
        Val_INT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //**********************************************************************
        //Lectura de Sensor de Movimiento 
        //**********************************************************************
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        Val_MOV = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //**********************************************************************
        //Lectura de Sensor de Vibración
        //**********************************************************************
        I2C_Master_Start();
        I2C_Master_Write(0x91);
        Val_VIB = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //**********************************************************************
        //Lectura de Sensor de Temperatura
        //**********************************************************************
        I2C_Master_Start();
        I2C_Master_Write(0xC1);
        Val_TEMP = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //**********************************************************************
        //Lectura de Sensor de Peso
        //**********************************************************************
        I2C_Master_Start();
        I2C_Master_Write(0xF1);
        Val_PESO = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //**********************************************************************
        // CONVERSIÓN DE DATOS LEÍDOS
        //******************************************************************************************
        // Proceso para obtener el estado del sensor de interferencia
        //******************************************************************************************
        if (Val_INT == 1){
            LCD_POINT(2,2);
            lcd_msg("OFF");
        }else{
            LCD_POINT(2,2);
            lcd_msg("ON");
        }
        //******************************************************************************************
        // Proceso para obtener el estado del sensor de movimiento
        //******************************************************************************************
        if (Val_MOV == 1){
            LCD_POINT(2,10);
            lcd_msg("ON");
        }else{
            LCD_POINT(2,10);
            lcd_msg("OFF");
        }
        //******************************************************************************************
        // Proceso para obtener el estado del sensor de vibracion
        //******************************************************************************************
        if (Val_VIB == 1){
            LCD_POINT(2,6);
            lcd_msg("ON");
        }else{
            LCD_POINT(2,6);
            lcd_msg("OFF");
        } 
        __delay_ms(2000);
        
        //******************************************************************************************
        // DESPLIEGUE DE SEGUNDO SET DE DATOS
        //******************************************************************************************      
        lcd_cmd(0x01);
    
        LCD_POINT(1,2);
        lcd_msg("TEMP"); //Se envía el string TEMP para indicar que los datos mostrados son del sensor de temperatura
        LCD_POINT(1,8);
        lcd_msg("PESO"); //Se envía el string S3 para indicar que los datos mostrados son del sensor de peso


        LCD_POINT(2,2);
        lcd_dwr('.'); //Se envía el caracter C a esa posición
        
        LCD_POINT(2,4);
        lcd_dwr(0b11011111);                               //Se envía el caracter correspondiente °
        LCD_POINT(2,5);
        lcd_dwr('C'); //Se envía el caracter C a esa posición
        LCD_POINT(2,11);
        lcd_dwr('g'); //Se envía el caracter g a esa posición
        
        //**********************************************************************
        //Conversión de Datos Analógicos
        //**********************************************************************
        /*
        ADC_TEMP_V = (float)((((float)710)-(Val_TEMP))/((float)6));             //Realiza la conversión de binario a valor de voltaje correspondiente
        */
        K = 1.00/(invT0 + invBeta*(log(adcMax/(float)Val_TEMP - 1.00)));
        C = K - 293.15;
        
        ADC_PESO_V = (float)((Val_TEMP)/((float)51));                           //Realiza la conversión de binario a valor de voltaje correspondiente
        //******************************************************************************************
        // Proceso para obtener dos enteros, y un decimal del dato del SENSOR DE TEMPERATURA
        //******************************************************************************************
        POT_cien = (float)((C)*((float)10));           //Se multiplica el valor de voltaje por cien -> 2.5456 => 254.6
        DECI_1_POT = (uint16_t)(POT_cien);                      //El valor anterior se vuelve entero -> 254.6 => 254
        POT_D2 = (uint8_t)((DECI_1_POT)%((uint8_t)10));         //Se realiza la división mod. obteniendo el decimal -> 254 => 4
        
        DECI_2_POT = (uint16_t)((DECI_1_POT)/((uint16_t)10));   //Se divide dentro de 10 el valor en entero -> 254 => 25
        POT_D1 = (uint8_t)((DECI_2_POT)%((uint8_t)10));         //Se realiza la división mod. obteniendo el primer decimal -> 25 => 5
        
        TEMP_EN_1 = (uint16_t)((DECI_2_POT)/((uint16_t)10));
        TEMP_EN = (uint8_t)((TEMP_EN_1)%((uint8_t)10));
        //POT_EN = (uint16_t)(TEMP_EN_1);                         //Se convierte el valor float a entero -> 2.546 => 2
        //**************************************************************************
        // Se envían cada uno de los digitos a la columna correspondiente de la LCD 
        //**************************************************************************
        LCD_POINT(2,0);
        lcd_dwr(Val_STR(TEMP_EN));                               //Se envía el caracter correspondiente a la decena        
        
        LCD_POINT(2,1);
        lcd_dwr(Val_STR(POT_D1));                               //Se envía el caracter correspondiente a la unidad
        
        LCD_POINT(2,3);
        lcd_dwr(Val_STR(POT_D2));                               //Se envía el caracter correspondiente al decimal
        /*
        //******************************************************************************************
        // Proceso para obtener el entero, y dos decimales del dato del SENSOR DE PESO
        //******************************************************************************************
        POT_cien = (float)((ADC_PESO_V)*((float)100));           //Se multiplica el valor de voltaje por cien -> 2.5456 => 254.6
        DECI_1_POT = (uint16_t)(POT_cien);                      //El valor anterior se vuelve entero -> 254.6 => 254
        POT_D2 = (uint8_t)((DECI_1_POT)%((uint8_t)10));         //Se realiza la división mod. obteniendo el segundo decimal -> 254 => 4
        
        DECI_2_POT = (uint16_t)((DECI_1_POT)/((uint16_t)10));   //Se divide dentro de 10 el valor en entero -> 254 => 25
        POT_D1 = (uint8_t)((DECI_2_POT)%((uint8_t)10));         //Se realiza la división mod. obteniendo el primer decimal -> 25 => 5
        
        POT_EN = (uint16_t)(ADC_PESO_V);                         //Se convierte el valor float a entero -> 2.546 => 2
        //**************************************************************************
        // Se envían cada uno de los digitos a la columna correspondiente de la LCD 
        //**************************************************************************
        LCD_POINT(2,8);
        lcd_dwr(Val_STR(POT_EN));                               //Se envía el caracter correspondiente al entero
                
        LCD_POINT(2,9);
        lcd_dwr(Val_STR(POT_D1));                               //Se envía el caracter correspondiente al primer decimal
        
        LCD_POINT(2,10);
        lcd_dwr(Val_STR(POT_D2));                               //Se envía el caracter correspondiente al segundo decimal
        */
        __delay_ms(2000);
        
        lcd_cmd(0x01);
        
        //**********************************************************************
        // COMUNICACIÓN UART - MANDA DATOS DE VARIABLES
        //**********************************************************************
        // Mandar datos de Sensor de Intereferencia
        //**********************************************************************
        UART_Write(Val_INT);
        __delay_ms(10);
        //**********************************************************************
        // Mandar datos de Sensor de Movimiento
        //**********************************************************************        
        UART_Write(Val_MOV);
        __delay_ms(10);
        //**********************************************************************
        // Mandar datos de Sensor de Vibración
        //**********************************************************************        
        UART_Write(Val_VIB);
        __delay_ms(10);
        //**********************************************************************
        // Mandar datos de Sensor de Temperatura
        //**********************************************************************        
        UART_Write(Val_TEMP);
        __delay_ms(10);
        //**********************************************************************
        // Mandar datos de Sensor de Peso
        //**********************************************************************        
        UART_Write(Val_PESO);
        __delay_ms(10);
    }   
    return;
}
//******************************************************************************
//Función de Inicialización de Puertos
//******************************************************************************
void init(void){
    TRISA = 0;                      // PORTA configurado como entrada en RA0
    TRISB = 0;                      // PORTB configurado como salida
    TRISC = 0;                      // PORTC configurado como salida
    TRISD = 0;                      // PORTD configurado como salida
    ANSEL = 0;                      // Pines connfigurados A0 y A3 como entradas analógicas
    ANSELH = 0;                     //Pines configurados como digitales 
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

//**************************************************************************************
//Función de relación entre número y caractér correspondiente para ser enviado a la LCD
//**************************************************************************************
uint8_t Val_STR(uint8_t num){ 
    switch(num){
        case 0: //Si el valor es cero, devuelva '0' para psoteriormente enviarlo a la LCD
            return '0';
            
        case 1: //Si el valor es uno, devuelva '1' para psoteriormente enviarlo a la LCD
            return '1';
            
        case 2: //Si el valor es dos, devuelva '2' para psoteriormente enviarlo a la LCD
            return '2';
            
        case 3: //Si el valor es tres, devuelva '3' para psoteriormente enviarlo a la LCD
            return '3';
            
        case 4: //Si el valor es cuatro, devuelva '4' para psoteriormente enviarlo a la LCD
            return '4';
            
        case 5: //Si el valor es cinco, devuelva '5' para psoteriormente enviarlo a la LCD
            return '5';
            
        case 6: //Si el valor es seis, devuelva '6' para psoteriormente enviarlo a la LCD
            return '6';
            
        case 7: //Si el valor es siete, devuelva '7' para psoteriormente enviarlo a la LCD
            return '7';
            
        case 8: //Si el valor es ocho, devuelva '8' para psoteriormente enviarlo a la LCD
            return '8';
            
        case 9: //Si el valor es nueve, devuelva '9' para psoteriormente enviarlo a la LCD
            return '9';      
            
    }
}
