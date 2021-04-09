/*
 * delay.c
 *
 * Created: 13/09/2018 11:40:50 a. m.
 *  Author: Patricio
 */ 

#include "Funciones_P1_E3.h"

void delay_us(unsigned long tiempo_us) {
    for (unsigned long i=0;i<tiempo_us;i++) {
        TCNT1H = 0xFF;
        TCNT1L = 0xF8; //8 Cuentas para delay de 1us
        TCCR1A = 0X00; //Timer1, Normal Mode
        TCCR1B = 0X01; //No prescaler
        while((TIFR&(0x1<<TOV1))==0); //Wait for TF0 to roll over
        TCCR1B = 0;
        TIFR = 0x1<<TOV1; //Clear TF0
    }
}
