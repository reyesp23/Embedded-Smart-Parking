/*
 * lcd2.c
 *
 * Created: 29/11/2018 03:12:00 p. m.
 *  Author: Patricio
 */


#include "Funciones_P1_E3.h"

void lcdCommand(unsigned char cmnd) {
    LCD_DPRT = cmnd;
    LCD_CPRT &= ~(1<<LCD_RS); //Registro de comandos
    LCD_CPRT &= ~(1<<LCD_RW); //Escritura
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~(1<<LCD_EN);
    _delay_us(100);
}

void lcdData(unsigned char data) {
    LCD_DPRT = data;
    LCD_CPRT |= (1<<LCD_RS);
    LCD_CPRT &= ~(1<<LCD_RW);
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~ (1<<LCD_EN);
    _delay_us(100);
}

void lcd_init() {
    //LCD_DDDR = 0xFF; //LCD DATA REGISTER: SALIDA
    //LCD_CDDR = 0xFF; //LCD COMMAND REGISTER: SALIDA
    LCD_CPRT &=~(1<<LCD_EN);
    _delay_us(2000);
    lcdCommand(0x38);
    lcdCommand(0x0E);
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
}

void lcd_gotoxy(unsigned char x, unsigned char y) {
    unsigned char firstCharAdrr[]={0x80, 0xC0, 0x94, 0xD4};
    lcdCommand(firstCharAdrr[y-1]+x-1);
    _delay_us(100);
}

void lcd_print(char * str) {
    unsigned char i = 0;
    while(str[i]!=0)
    {
        lcdData(str[i]);
        i++;
    }
}
