/*
 * CFile1.c
 *
 * Created: 29/11/2018 03:36:59 p. m.
 *  Author: Patricio
 */

#include "Funciones_P1_E3.h"

void rutina_AUTH(unsigned char LOT_STATUS[AV_SPOTS]) {
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
    lcd_gotoxy(1,1);
    lcd_print("Autorizado");
    
    unsigned char COUNTER = AV_SPOTS; //LUGARES DISPONIBLES EN EL ESTACIONAMIENTO
    for (unsigned char i=0;i<AV_SPOTS;i++) { //POR CADA LUGAR
        if (LOT_STATUS[i]==0){ //SI ESTA OCUPADO, RESTAR UNO AL CONTADOR
            COUNTER = COUNTER - 1;
        }
    }
    _delay_us(500000);
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
    lcd_gotoxy(1,1);
    lcd_print("Disponibles:");
    
    if (COUNTER == 1) {
        lcd_gotoxy(14,1);
        lcd_print("1");
    }
    if (COUNTER == 2) {
        lcd_gotoxy(14,1);
        lcd_print("2");
    }
    if (COUNTER == 3) {
        lcd_gotoxy(14,1);
        lcd_print("3");
    }
    lcd_gotoxy(1,2);
    lcd_print("A1   A2   A3");
    _delay_us(500000);
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
    
    //Servo PWM
    unsigned char i = 33;
    while (i < 64) {
        OCR2 = i;
        _delay_us(20000);
        i++;
    }
    _delay_us(600000);
    while (i > 32) {
        OCR2 = i;
        _delay_us(20000);
        i--;
    }
}

void rutina_DENIED() {
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
    lcd_gotoxy(1,1);
    lcd_print("Denegado");
    _delay_us(500000);
    lcdCommand(0x01);
    _delay_us(2000);
    lcdCommand(0x06);
}
