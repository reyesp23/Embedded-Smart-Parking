/*
 * checkTag.c
 *
 * Created: 29/11/2018 03:36:20 p. m.
 *  Author: Patricio
 */ 
#include "Funciones_P1_E3.h"

unsigned char AUTH_TAG[14]={2,56,56,48,48,49,48,48,55,53,54,67,57,3};

unsigned char checkTag(unsigned char NEW_TAG[14]) {
    unsigned char VERIF_COUNTER = 0; //Contador de verificacion
    
    for(int i = 0; i<14; i++) { //Para cada elemento del ID
        if (NEW_TAG[i]==AUTH_TAG[i]) { //Comparar la tarjeta leida con las tarjetas autorizadas
            VERIF_COUNTER++; //Si es el dato es igual, aumentar un contador
        }
    } //Termina ciclo for
    
    if(VERIF_COUNTER == 14) { //Si el contador es igual a 14, todos los valores coinciden, Autorizado
        return 1; //Regresar autorizado
    } else {
        return 0; //Regresar denegado
    }
}
