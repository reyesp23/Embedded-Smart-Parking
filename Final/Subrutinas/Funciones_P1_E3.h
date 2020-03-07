/*
 * Funciones_P1_E3.h
 *
 * Created: 13/09/2018 11:41:24 a. m.
 *  Author: Patricio
 */ 


#ifndef FUNCIONES_P1_E3_H_
#define FUNCIONES_P1_E3_H_
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define AV_SPOTS 3

//INDICADORES DE LUGAR
#define LEDs_PORT PORTB
#define LED_A1_RED 0
#define LED_A1_GREEN 1	
#define LED_A2_RED 2	
#define LED_A2_GREEN 3	
#define LED_A3_RED 4	
#define LED_A3_GREEN 5	
#define LED_STK_0 6
#define LED_STK_1 7

//LCD
#define LCD_DPRT PORTA		//LCD DATA PORT
#define LCD_DDDR DDRA		//LCD DATA DDR
#define LCD_DPIN PINA		//LCD DATA PINS

#define LCD_CPRT PORTC		//LCD COMMANDS PORT
#define LCD_CDDR DDRC		//LCD COMMANDS PIN
#define LCD_CPIN PINC
#define LCD_RS 7
#define LCD_RW 6
#define LCD_EN 0

//PORT D: VARIOS

#define RX 0
#define MUX_A 1
#define MUX_B 2
#define TRIGGER_S1 3
#define TRIGGER_S2 4
#define TRIGGER_S3 5



//void delay_us(unsigned long tiempo_us);

void lcdCommand(unsigned char cmnd);

void lcdData(unsigned char data);


	
void lcd_init();


void lcd_gotoxy(unsigned char x, unsigned char y);

void lcd_print(char * str);

void rutina_AUTH(unsigned char LOT_STATUS[AV_SPOTS]);
void rutina_DENIED();
void rutina_AllBusy();
unsigned char checkTag(unsigned char NEW_TAG[14]);


#endif /* FUNCIONES_P1_E3_H_ */