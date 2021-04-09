/*
 * Proyecto1_E3.c
 *
 * Created: 13/09/2018 11:38:44 a. m.
 * Author : Patricio
 */

#include "Subrutinas/Funciones_P1_E3.h"

unsigned char NEW_RFID_READ = 0;
unsigned char NEW_TAG[14];
unsigned char CHAR_COUNTER=0;
unsigned char GOT_TAG = 0;
unsigned char T0_COUNTER_50 = 0; //El contador llego a 50 = 500ms 

unsigned char LOT_STATUS[AV_SPOTS] = {1,1,1};
unsigned char SPOT_COUNTER = 3;
unsigned char ALL_BUSY_FLAG = 0;

int FIRST_MEASUREMENT_FLAG=0; //Se realizo la primera medida del ICU
int SECOND_MEASUREMENT_FLAG=0; //Se realizo la segunda medida del ICU
unsigned int t; //Guarda el periodo de la señal leida en el ICU
unsigned int T0_COUNTER_20 = 0; //El contador llego a 10 = 100ms
unsigned char CHECK_S1 = 1;
unsigned char CHECK_S2 = 0;
unsigned char CHECK_S3 = 0;

int main(void) {
    
    //Inicializacion
    DDRC = 0xFF; //Puerto C de salida (Control LCD)
    DDRA = 0xFF; //Puerto A de salida LCD (Data)
    DDRD = 0b10111110; //D7 salida (PWM OUT). D1-D2 (MUX Select), D3-D5 (SENSOR TRIGGERS), D0 (SERIAL IN)
    DDRB = 0xFF; //Puerto B de salida (LEDs)
    
    //Interrupcion Timer0 (Contar tiempo: 10ms)
    TCCR0 = 0x05; //Modo normal, pre-scaler 1024
    TCNT0 = 0xB2; //Cuenta para 10ms
    
    //Interrupcion Timer1 (Input Capture Unit)
    TCCR1A = 0; //Modo normal
    TCCR1B = 0x41; // Modo
    
    TIMSK = (1<<TICIE1)| (1<<TOIE0) ; //Habilita Interrupts: ICU de T1
    
    //Timer 2 Fast PWM - Servo
    TCCR2 = 0x6E; //Fast PWM, non invertido, pre-scaler 256
    OCR2=33; //Duty Cycle 12% para 1ms = 0
    
    //Interrupcion Serial
    //UCSRB = (1<<RXEN) | (1<<RXCIE) | (1<<TXEN) | (1<<UDRIE);
    UCSRB = (1<<RXEN) | (1<<RXCIE); //Habilitar RX y RX Complete Interrupt
    UCSRC = (1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL)|(0<<UMSEL); //Configurar Serial; Asincrono, 1 bit de stop, 8
    UBRRL = 0X33;
    sei(); //Habilitar interrupciones globales
    
    lcd_init(); //Inicializar display
    
    while (1) {
        if (ALL_BUSY_FLAG == 0) {//Mientras aun hay lugares disponibles
            lcd_gotoxy(1,1);
            lcd_print("Bienvenido"); //Mostrar mensaje inicial
            if (NEW_RFID_READ == 1) { //Lectura de una nueva tarjeta
                if (checkTag(NEW_TAG)) {
                    rutina_AUTH(LOT_STATUS);
                } else {
                    rutina_DENIED();
                } //TERMINA CHECK NEW TAG
                T0_COUNTER_20 = 0; //Reiniciamos contador para continuar con el polling
                NEW_RFID_READ = 0; //Termina la lectura de la nueva tarjeta
                UCSRB = (1<<RXEN) | (1<<RXCIE); //Se habilita RX y su interrupcion para leer nuevas tarjetas
            } //Termina la rutina de lectura de tarjeta
        } else { //Termina la bandera de ALL BUSY
            lcd_gotoxy(1,1);
            lcd_print("Cupo Lleno");
            CHAR_COUNTER = 0;
        }
        /****************** CHEQUEO DE LUGARES ******************************/
        
        //********* POLLING DE SENSORES ***********/
        
        if(T0_COUNTER_20==50) {//Polling cada 200ms
            T0_COUNTER_20=0; //Reiniciar contador
            
            if (CHECK_S1) {
                //Seleccion de MUX
                PORTD&=~(1<<1);
                PORTD&=~(1<<2);
                TCCR1B = 0x41; //Rising edge, no pre-scaler Input Capture mode
                //Pulso de 10us al trigger
                PORTD |= (1 << 3);
                _delay_us(10);
                PORTD &= (~(1 << 3));
            } //TERMINA: CHECK S1
            
            else if (CHECK_S2) {
                //Seleccion de MUX
                PORTD|=(1<<1);
                PORTD&=~(1<<2);
                TCCR1B = 0x41; //Rising edge, no pre-scaler Input Capture mode
                //Pulso de 10us al trigger
                PORTD |= (1 << 4);
                _delay_us(10);
                PORTD &= (~(1 << 4));
                
            } //TERMINA: CHECK S2
            else if (CHECK_S3) {
                //Seleccion de MUX
                PORTD|=(1<<1);
                PORTD|=(1<<2);
                TCCR1B = 0x41; //Rising edge, no pre-scaler Input Capture mode
                //Pulso de 10us al trigger
                PORTD |= (1 << 5);
                _delay_us(10);
                PORTD &= (~(1 << 5));
            } //TERMINA: CHECK S3
        } //Termina contador
        
        if (SECOND_MEASUREMENT_FLAG) {
            SECOND_MEASUREMENT_FLAG = 0;
            if (t>6000) { //Lugar libre
                if (CHECK_S1) {
                    LOT_STATUS[0] = 1;
                } else if (CHECK_S2) {
                    LOT_STATUS[1] = 1;
                } else if (CHECK_S3) {
                    LOT_STATUS[2] = 1;
                }
            }
            else { //Lugar ocupado
                if (CHECK_S1) {
                    LOT_STATUS[0] = 0;
                } else if (CHECK_S2) {
                    LOT_STATUS[1] = 0;
                } else if (CHECK_S3) {
                    LOT_STATUS[2] = 0;
                }
            }
            
            if (CHECK_S1) {
                CHECK_S1 = 0;
                CHECK_S2 = 1;
                CHECK_S3 = 0;
            } else if (CHECK_S2) {
                CHECK_S1 = 0;
                CHECK_S2 = 0;
                CHECK_S3 = 1;
            } else if (CHECK_S3) {
                CHECK_S1 = 1;
                CHECK_S2 = 0;
                CHECK_S3 = 0;
            }
        }
        //******************* ACTUALIZACION DE LA INFORMACION DISPONIBLE
        if (LOT_STATUS[0]) { // Si el STATUS es 1, el LED esta encendido
            LEDs_PORT &= ~(1<<LED_A1_RED); //Red LED OFF
            LEDs_PORT |= (1<<LED_A1_GREEN); //Green LED ON
            
        } else { //Si el STATUS es de 0, el LED esta apagado
            LEDs_PORT &= ~(1<<LED_A1_GREEN); //GREEN LED OFF
            LEDs_PORT |= (1<<LED_A1_RED); //RED LED ON
        }
        
        if (LOT_STATUS[1]){ // Si el STATUS es 1, el LED esta encendido
            LEDs_PORT &= ~(1<<LED_A2_RED); //Red LED OFF
            LEDs_PORT |= (1<<LED_A2_GREEN); //Green LED ON
            
        } else { //Si el STATUS es de 0, el LED esta apagado
            LEDs_PORT &= ~(1<<LED_A2_GREEN); //GREEN LED OFF
            LEDs_PORT |= (1<<LED_A2_RED); //RED LED ON
        }
        
        if (LOT_STATUS[2]) { // Si el STATUS es 1, el LED esta encendido
            LEDs_PORT &= ~(1<<LED_A3_RED); //Red LED OFF
            LEDs_PORT |= (1<<LED_A3_GREEN); //Green LED ON
            
        } else { //Si el STATUS es de 0, el LED esta apagado
            LEDs_PORT &= ~(1<<LED_A3_GREEN); //GREEN LED OFF
            LEDs_PORT |= (1<<LED_A3_RED); //RED LED ON
        }
        
        //CONTADOR DE LUGARES
        SPOT_COUNTER = AV_SPOTS; //LUGARES DISPONIBLES EN EL ESTACIONAMIENTO
        for (unsigned char i=0;i<AV_SPOTS;i++) { //POR CADA LUGAR
            if (LOT_STATUS[i]==0) { //SI ESTA OCUPADO, RESTAR UNO AL CONTADOR
                SPOT_COUNTER = SPOT_COUNTER - 1;
            }
        }
        
        //VERIFICAR SI HAY LUGARES DISPONIBLES
        if (SPOT_COUNTER == 0) { //EL CONTADOR DE LUGARES LLEGO A 0
            ALL_BUSY_FLAG = 1;
        } else {
            ALL_BUSY_FLAG = 0; //EL CONTADOR DE LUGARES ES MAYOR A 0, AUN HAY LUGARES DISPONIBLES
        }
    } // Termina ciclo while 1
}

ISR(USART_RXC_vect) { //Interrupcion serial de recepcion de datos (RFID)
    if (CHAR_COUNTER<14) { //Aun faltan datos de la tarjeta
        NEW_TAG[CHAR_COUNTER] = UDR; //Guarda el ultimo dato obtenido
        CHAR_COUNTER++; //Incrementa la cuenta de datos
    } else { //Se obtuvieron los 14 numeros de ID
        NEW_RFID_READ = 1; //Bandera de nueva lectura de RFID
        CHAR_COUNTER = 0; //Reseteamos la cuenta de data
        UCSRB = (0<<RXEN) | (0<<RXCIE); //Deshabilitamos RX e interupciones para leer una tarjeta a la vez
    }
}

ISR(TIMER1_CAPT_vect) {
    if (FIRST_MEASUREMENT_FLAG==0) { //Aun no se toma la primera medida
        t=ICR1; //Guardar la primera medida
        TCCR1B = 0x01;		//Poner en modo falling edge para esperar la bajada
        FIRST_MEASUREMENT_FLAG = 1; //Primera medida ya se tomo
    }
    else { //Ya se tomo la primera medida
        TCCR1B = 0x41; //Regresar a modo rising edge
        t = ICR1-t; //Tomar la segunda medida y restarle la primera medida
        FIRST_MEASUREMENT_FLAG=0; //Primera medida = 0
        SECOND_MEASUREMENT_FLAG=1; //Segunda medida ya se tomo
    }
}

ISR(TIMER0_OVF_vect) { //Cuenta 10ms
    TCNT0 = 0xB2; //Carga el valor del Timer nuevamente
    T0_COUNTER_20++; //Incrementa contador del blinking
}
