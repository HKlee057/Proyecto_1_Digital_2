/*
 * File:   ADC.c
 * Author: Hector Klée
 *
 * Created on 5 de febrero de 2020, 02:48 PM
 */

#include <xc.h>
#include<stdio.h>
#include <stdint.h>
#include "ADC.h"

#define _XTAL_FREQ 8000000

void initADC(void){
    
    ADCON0bits.ADCS0 = 0;       //Reloj de conversión a FOSC/8 
    ADCON0bits.ADCS1 = 1;
    
    ADCON1bits.ADFM = 0;        //Valor justificado a la izquierda
    ADCON1bits.VCFG0 = 0;       //Voltaje de referencia positivo en 5V
    ADCON1bits.VCFG1 = 0;       //Voltaje de referencia negativo en Tierra
   
    ADCON0bits.ADON = 1;       //El ADC está habilitado
}


