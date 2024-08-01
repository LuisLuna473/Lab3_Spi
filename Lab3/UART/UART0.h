/*
 * UART0.h
 *
 * Created: 25/07/2024 19:42:21
 *  Author: luisa
 */ 


#ifndef UART0_H_
#define UART0_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <stddef.h>

/*
	DEFINICIONES DE VARIABLES
*/
#define M9600 1
#define BAUD_PRESCALE(baud) ((F_CPU / (baud * 16UL)) - 1)

volatile char uart_buffer[128]; // Buffer circular para almacenar datos recibidos por UART
volatile uint8_t uart_head; // Índice de la cabeza del buffer
volatile uint8_t uart_tail; // Índice de la cola del buffer

//Modo 9600
void SetupUART0(uint8_t MODO);
void writeTextUART(char * Texto);
void writeUART(char Caracter);
void procesarCadena(char *cadena_mc);
void recibir(char dato); 

/* Verifica si hay datos disponibles en el buffer UART. */
int UART_available(void);

/* Lee una cadena del buffer UART hasta que se encuentra un delimitador. */
void UART_receive_string(char* buffer, uint8_t max_length);

/* Lee un solo carácter del buffer UART. */
unsigned char UART_receive(void);

void UART_send_string(const char* str);

/* Envía un número a través de UART. */
void UART_send_number(uint8_t number);



#endif /* UART0_H_ */