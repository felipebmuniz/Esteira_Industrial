/*
 * File:   global.h
 * Author: felip
 *
 * Created on 29 de Novembro de 2021, 00:23
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include <xc.h> // include processor files - each processor file is guarded.

// CONFIGURAÇÕES GERAIS DO CHIP
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//FREQUENCIA DO OCILADOR
#define _XTAL_FREQ  20000000

// Recarga do timer 0
#define rec_timer0 1

//PINOS DO MICRO
#define sensor_A PORTBbits.RB4
#define sensor_B PORTBbits.RB5
#define botao1 PORTBbits.RB6

#define led_motor1 PORTCbits.RC2
#define led_motor2 PORTCbits.RC1
#define led_alerta PORTCbits.RC0

/* LCD: Always define: */
#define LCD_nbits 4             // number of LCD data bits
#define LCD_RS	PORTDbits.RD0   // LCD command/character selection
#define LCD_EN	PORTDbits.RD1   // LCD enable pin


/* LCD: define on 4 bits mode: */
#define LCD4	PORTDbits.RD4   // bit 4 of lcd
#define LCD5	PORTDbits.RD5   // bit 5 of lcd
#define LCD6	PORTDbits.RD6   // bit 6 of lcd
#define LCD7	PORTDbits.RD7   // bit 7 of lcd


unsigned char option;

// contador de numeros de ciclos
unsigned int count0 = 0; // PARA TIMER 0

unsigned int pacotes = 0; // CONTADOR DE PACOTES

unsigned int rm_pacote = 0; // CONTADOR DE PACOTES

unsigned int ps_c = 0; // Verificar dualidade de envio

unsigned char char_value[5]; // receive char value

#endif	/* GLOBALS_H */