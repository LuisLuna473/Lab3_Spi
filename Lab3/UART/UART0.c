/*
 * UART0.c
 *
 * Created: 25/07/2024 19:42:29
 *  Author: luisa
 */ 
#include "UART0.h"

void SetupUART0(uint8_t MODO)
{
	if (MODO == M9600){
		//Paso 1: RX como entrada y TX como salida
		DDRD &= ~(1<<DDD0);
		DDRD |= (1<<DDD1);
		
		//Paso 2: Confi. UCSR0A
		UCSR0A = 0;
		
		//Paso 3: Conf. UCSR0B > donde habilitamos ISR de recepción, habilitamos rx y tx
		UCSR0B = 0;
		
		UCSR0B |= (1<< RXCIE0) |(1<<RXEN0) |(1<<TXEN0);
		
		//Paso 4: Cond. UCSR0C > Asyncrono, Paridad: None, 1 bit de Stop, Data bits 8/bits
		UCSR0C = 0;
		
		UCSR0C |= (1<< UCSZ01)| (1<< UCSZ00);
		
		//Paso #5: Conf. velocidad de Baudrate: 9600
		
		UBRR0 = 103;
		
	}
}

/* Envía una cadena de caracteres a través de UART. */
void UART_send_string(const char* str) {
	while (*str) {
		writeUART(*str++); // Enviar cada carácter de la cadena
	}
}

void writeTextUART(char * Texto){
	
	for (int i=0; Texto[i]!='\0'; i++){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = Texto[i];
	}
}

void writeUART(char Caracter){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}

void recibir(char dato){
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

/* Lee un solo carácter del buffer UART. */
unsigned char UART_receive(void) {
	if (uart_head != uart_tail) { // Verificar si hay datos disponibles
		unsigned char data = uart_buffer[uart_tail]; // Leer el carácter del buffer
		uart_tail = (uart_tail + 1) % sizeof(uart_buffer); // Actualizar el índice de la cola
		return data; // Devolver el carácter recibido
		} else {
		return 0; // No hay datos disponibles
	}
}

/* Lee una cadena del buffer UART hasta que se encuentra un delimitador. */
void UART_receive_string(char* buffer, uint8_t max_length) {
	uint8_t index = 0;
	char received_char;

	while (index < (max_length - 1)) { // Deja espacio para el terminador de cadena
		if (UART_available()) {
			received_char = UART_receive();
			if (received_char == '\n' || received_char == '\r') {
				// Fin de la cadena
				break;
			}
			buffer[index++] = received_char;
		}
	}
	buffer[index] = '\0'; // Null-terminar el string
}

/* Verifica si hay datos disponibles en el buffer UART. */
int UART_available(void) {
	return (uart_head != uart_tail); // Retorna 1 si hay datos en el buffer, 0 si no
}

/* Envía un número a través de UART. */
void UART_send_number(uint8_t number) {
	char buffer[4]; // Para almacenar hasta 3 dígitos y un terminador de cadena
	snprintf(buffer, sizeof(buffer), "%u", number); // Convierte el número a cadena
	UART_send_string(buffer); // Envía la cadena
}
