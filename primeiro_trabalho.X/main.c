/*
 * File:   main.c
 * Author: felip
 *
 * Created on 28 de Novembro de 2021, 12:45
 */

//INCLUDES DE ARQUIVOS
#include <stdlib.h>
#include "global.h"
#include "lcd.h"

//FUNÇÕES

void init(void) {
    PORTA = 0b00000000;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000000;
    PORTE = 0b00000000;

    // IO
    TRISA = 0b00010000;
    TRISB = 0b01110000; // BITS DE ENTRADA E SAIDA
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    TRISE = 0b00000000;

    // INT->Interrupção && TMR->Timer 0

    //CONFIGURAÇÂO PARA TMR0 ->
    // COM PRESET EM 000 E WHACHTDOG EM 1 IRÁ PRECISAR SOMENTE DE UMA MUDANÇA
    // DE NIVEL LOGICO DO CLOCK EXTERNO
    OPTION_REG = 0b10101000;
    //             | |  |||
    //             | |  000->prescaler-> define contagem de 1 ciclos de maquina
    //             | |-> 1 -> seleção de clock, 1 clock esterno entrada RA4
    //             1->Pull-up ativo

    // HABITAR INTERRUPÇÂO, TIMER 0 e INTERRUPÇÃO DE RB4-RB7
    INTCON = 0b00111000;
    //           |||-> Ativação das portas RB4-RB7 para interrupção
    //           ||-> 1-> INTERUPÇÃO ativa
    //           |-> 1->  TIMER0 ativo -> de 0 á 256 bits
    

    // DEFINIÇÃO DA INICIALIZAÇÃO DO TIMER 0
    TMR0 = (256 - rec_timer0);

    // DESABILITAR OS MODULOS COMPARADORES - CCP
    CMCON = 0x07;
}

void pac_A(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("PAC EM A       ", 15);
}

void pac_B(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("PAC EM B       ", 15);
}

void pac_C(void) {
    lcd_goto(1, 1);
    lcd_puts("PAC PASSOU EM C", 15);
}

void pac_Erro(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("REMOVER 1 PAC  ", 15);
}

void pac_Remova(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("REMOVER PAC    ", 15);
}

void esperando(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("ESPERANDO PAC ", 15);
}

void ligar_motor(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("LIGUE O MOTOR  ", 15);
}

void pac_null(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("NENHUM PAC     ", 15);
}

void pac_dual(void) {
    option = ' ';
    lcd_goto(1, 1);
    lcd_puts("TERMINE ENVIO  ", 15);
}

void qnt(void) {
    lcd_goto(2, 1);
    lcd_puts("QNT PAC: 0", 10);
}

void set_valor(unsigned int valor) {
    // CONVERTE INTEIRO PARA VETOR DE CHAR
    itoa(char_value, valor, 10); 
    // LCD
    // write LCD
    lcd_goto(2, 10);
    lcd_puts(char_value, 3);
}

void __interrupt() myIsr() {
    //Configuração da interrupção

    if (INTCONbits.RBIE && INTCONbits.RBIF) {
        INTCONbits.RBIF = 0;

        // VERIFICA SE A DOIS PACOTES NA ESTEIRA NAS ESTREMIDADES
        if (sensor_A == 1 && sensor_B == 1) {
            option = 'E'; // PRESETE DE MENSAGEM DE ERRO
            
            // ATIVA SE UM SENSOR ESTIVER LIGADO
        } else if (sensor_A == 1 || sensor_B == 1) {

            // INDENTIFICA SE TEM PACOTE NA ESTREMIDADE DO SENSOR A
            if (sensor_A == 1 && rm_pacote == 0) {
                option = 'A'; // PRESETE PARA MENSDAGEM DE PACOTE EM A
                
                // LIGA O MOTOR SE A ORDEM FOR DADA E NÂO TIVER PACOTE EM B
                if (botao1 == 1 && sensor_B == 0 && led_motor2 == 0) {
                    led_motor1 = 1; // LIGA MOTOR 1
                }

                // INDENTIFICA SE TEM PACOTE NO SENTIDO DO SENSOR DE A -> B
            } else if (led_motor1 == 1 && sensor_B == 1) {
                led_alerta = 1; // ALERTA PARA REMOÇÂO
                led_motor1 = 0; // DESLIGA MOTOR 1 
                rm_pacote = 1;  // VARIAVEL AUXILIAR DE VERIFICAÇÃO DE REMOÇÂO
                option = 'R';   // PRESTE PARA MENSAGEM DE REMOÇÂO
            }

            // LOGICA DO SENSOR B
            // INDENTIFICA SE TEM PACOTE NA ESTREMIDADE DO SENSOR A
            if (sensor_B == 1 && rm_pacote == 0) {
                option = 'B';// PRESETE PARA MENSDAGEM DE PACOTE EM B
                
                // LIGA O MOTOR SE A ORDEM FOR DADA E NÂO TIVER PACOTE EM A
                if (botao1 == 1 && sensor_A == 0 && led_motor1 == 0) {
                    led_motor2 = 1; // LIGA MOTOR 2
                }
                
                // INDENTIFICA SE TEM PACOTE NO SENTIDO DO SENSOR DE B -> A
            } else if (led_motor2 == 1 && sensor_A == 1 && rm_pacote == 0) {
                led_alerta = 1; // ALERTA PARA REMOÇÂO
                led_motor2 = 0; // DESLIGA MOTOR 2
                rm_pacote = 1;  // VARIAVEL AUXILIAR DE VERIFICAÇÃO DE REMOÇÂO
                option = 'R';   //PRESTE PARA MENSAGEM DE REMOÇÂO
            }
            
            // DESLIGA MAQUINA SEM ORDEM DE SERVIÇO
        } else if (sensor_A == 0 && sensor_B == 0 && botao1 == 0) {
            led_motor1 = 0; // DESLIGA MOTOR 1
            led_motor2 = 0; // DESLIGA MOTOR 2
            led_alerta = 0; // DESLIGA ALARME
            option = 'P';   // PRESETE DE MENSAGEM DE ESPERA DE PACOTE
            
            // VERIFIVAÇÃO DA REMOÇÂO DO PACOTE 
        } else if (sensor_A == 0 && sensor_B == 0 && botao1 == 1 && rm_pacote == 1) {
            led_alerta = 0; // DESLIGA ALARME
            rm_pacote = 0;  // REMOVE PACOTE DA VARIAVEL AXCILIAR
            ps_c = 0;       // VARIAVEL AUXILIAR DE ERRO DE MULTIPLO ENVIO
            option = 'P';   // PRESETE DE MENSAGEM DE ESPERA DE PACOTE
        }

        // RESET DE FUNCIONAMENTO DA MAQUIDA SEM ORDEM DE SERVIÇO
        if (botao1 == 0) {
            led_motor1 = 0; // DESLIGA MOTOR 1
            led_motor2 = 0; // DESLIGA MOTOR 2
            led_alerta = 0; // DESLIGA ALARME
        }
    }

    //Configuração do timer 0
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        // COM O WATCHDOG E PRESCALER PARA CONTAR A CADA UMA INTERAÇÃO
        TMR0 = (256 - rec_timer0) + TMR0; // VERIFICA O CLOCK EXTERNO
        INTCONbits.TMR0IF = 0;
        count0++; // DEFINE NUMERO DE CLOCK
    }
}

void main(void) {
    INTCONbits.GIE = 0;

    init(); // INICIA O SISTEMA

    INTCONbits.GIE = 1;

    // initialize LCD
    lcd_init();

    // write LCD
    esperando();
    qnt();

    // MENU DE MENSAGEM DO LCD
    while (1) {
        switch (option) {
            case 'A':
                pac_A(); // PACOTE EM A
                break;
            case 'B':
                pac_B(); // PACOTE EM B
                break;
            case 'E':
                pac_Erro(); // MULTIPLOS PACOTES
                break;
            case 'R':
                pac_Remova(); // REMOVER PACOTE
                break;
            case 'P':
                esperando(); // ESPERANDO PACOTE
                break;
            default:
                break;
        }

        if (count0 == 1) {
            
            // VERIFICA SE TEM ORDEM DE ENVIO
            if (botao1 == 0) {
                // MENSAGEM DE LIGAR MOTOR
                ligar_motor();
                
                // VERIFICA SE TEM PACOTE
            } else if (sensor_A == 1 || sensor_B == 1) {
                
                // VERIFICA MULTIPLOS ENVIOS
                if (ps_c == 0) {
                    ps_c++; // ENVIO PASSOU POR SENSOR C
                    pacotes++; // CONTADOR DE PACOTES ENVIADOS
                    pac_C(); // MENSAGEM DE PASSAGEM POR C

                    // A CADA 5 PACOTES MOSTRAR NO LCD
                    if ((pacotes % 5) == 0 && pacotes != 0) {
                        set_valor(pacotes); // MENSGEM DE QUANTIDADE DE PACOTES
                    }
                } else {
                    pac_dual(); // MENSAGEM DE DUALIDADE DE ENVIO
                }
            } else {
                pac_null(); // MENSAGEM DE NENHUM PACOTE
            }
            count0 = 0; // ZERA O CLOCK
        }
    }
    return;
}