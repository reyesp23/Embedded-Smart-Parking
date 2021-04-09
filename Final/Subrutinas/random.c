/*
 * random.c
 *
 * Created: 14/09/2018 11:58:34 a. m.
 *  Author: Patricio
 */ 
#include "Funciones_P1_E3.h"

unsigned char generate_random() {
    unsigned char led_num = 1;
    for(int z=0;z<(rand() % 8);z++) { //Eleva 2 a la potencia n (0-7)
        led_num = led_num*2;
    }
    return led_num; //Regresa el valor random
}
