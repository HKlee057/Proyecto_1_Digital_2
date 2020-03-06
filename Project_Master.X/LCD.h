/* 
 * File:                LCD 16X2  
 * Author:              Hector Klée
 * Comments:            Módulo de control de LCD
 * Revision history: 
 */

/*El código a continuación, y en el archivo .c correspondiente toman inspiración 
 del código proporcionado por Pablo Mazariegos en el siguiente enlace:
 https://github.com/pdmazariegos-uvg/ie3027/tree/master/Ejemplos/LCD4Bits.X*/ 
#ifndef __LCD_H_
#define	__LCD_H_

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RD7
#endif

#ifndef RW
#define RW RD2
#endif

#ifndef EN
#define EN RD3
#endif

#ifndef LCD_pin
#define LCD_pin PORTB
#endif

#include <xc.h> // include processor files - each processor file is guarded.  

void lcd_cmd(unsigned char x);
void lcd_dwr(unsigned char x);
void lcd_msg(unsigned char *c);
void lcd_ready(void);
void lcd_lat(void);
void lcd_init(void);
void LCD_POINT (uint8_t lin, uint8_t col);


#endif	/* LCD_H */

