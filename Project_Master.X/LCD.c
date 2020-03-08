/*
 * File:   LCD.c
 * Author: Hector Klée
 *
 * Created on 5 de febrero de 2020, 02:48 PM
 */
#include <stdint.h>
#include "LCD.h"

#define _XTAL_FREQ 8000000
//#define RS PORTDbits.RD2
//#define EN PORTDbits.RD3
//#define DISPLAY PORTA 

void lcd_cmd(unsigned char x){     //Función de comando de LCD
    lcd_ready();
    LCD_pin = x;
    RS = 0;
    RW = 0;
    lcd_lat();
}

void lcd_lat(){                 //Función de CLEAR
    EN = 1;
   __delay_ms(30);
   EN = 0;
}
void lcd_ready(void) {          //Función que prerpara el LCD para desplegar dato
    LCD_pin = 0xFF;
    LCD_pin &= 0x80;
    RS = 0;
    RW = 1;
    EN = 0;
    __delay_ms(30);
    EN = 1;
    if (LCD_pin == 0x80){
    EN = 0;
    __delay_ms(30);
    EN = 1;  
    }
    else{
        
    }
    
    
}

void lcd_dwr(unsigned char x){
    lcd_ready();
    LCD_pin = x;
    RS = 1;
    RW = 0;
    lcd_lat();
  
  
}

void lcd_msg(unsigned char *c){             //Función de despliegue de valor
    
    while(*c != 0)
        lcd_dwr(*c++);
}

void lcd_init(void){                        //Función de inicialización
    
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x06);
    lcd_cmd(0x80);
    
}

//******************************************************************************

//Función para indicar la posición del cursor

//******************************************************************************
void LCD_POINT (uint8_t lin, uint8_t col){ //Basado en la función hecha por Bithawa Bindu
                                           // Extraído de https://www.studentcompanion.co.za/interfacing-lcd-display-with-pic-microcontroller-xc8/
    if((lin > 2)||(col> 31)){ //Si el valor dado de columnas y filas es mayor al permitido, no haga algo
        return; 
    }else{ //Sino realiza un or entre 0X80 y col. si la linea es 1, y entre C0 y columnas si la fila es 2
        lcd_cmd((lin == 1) ? (0x80|col): (0xC0|col));
        __delay_ms(5); //Espere 5 ms
    }
}




