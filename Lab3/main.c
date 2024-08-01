/*
 * Lab3.c
 *
 * Created: 25/07/2024 19:31:58
 * Author : luisa
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "UART/UART0.h"
#include "SPI/SPI.h"

void refreshPORT(uint8_t valor); 
volatile char buffer[16];

volatile uint8_t valorSPI1 = 0;
volatile uint8_t valorSPI2 = 0; 
int valorSpi1b, valorSpi2b;
int reeived_value = 0;
volatile uint8_t valorUART = 0; // Variable para almacenar el valor recibido por UART
volatile char bufferUART[4];    // Buffer para almacenar los caracteres recibidos
volatile uint8_t indexUART = 0; // Índice para el buffer
char input_buffer[4]; // Buffer para almacenar la cadena recibida
int received_value = 0; 
uint8_t inicio = 1;

void sendUART(void);
void IntCont();
void llegadaADCS(void);
void contNUm(); 


int main(void)
{
	DDRC |= (1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5);
	DDRD |= (1<<DDD2)|(1<<DDD3); 
	
	PORTC &= ~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
	PORTD &= ~((1<<PORTD2)|(1<<PORTD3));
	
	spiInit(SPI_MASTER_OSC_DIV2, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
    SetupUART0(M9600);
	
    while (1) 
    {
		recibirDatoUART();
		
		refreshPORT(valorUART);
		if (inicio == 1) {
			_delay_ms(100); // Esperar un tiempo para permitir la actualización del ADC
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			llegadaADCS();
			_delay_ms(300);
			IntCont();
			inicio = 0;
			} else {
			_delay_ms(500); // Esperar un tiempo para permitir la actualización del ADC
			IntCont(); // Verificar y procesar los comandos UART
		}
		llegadaADCS(); // Leer ADC y actualizar valores
		_delay_ms(300);
    }
}

void sendUART(void){
	valorSpi2b = (int)(valorSPI2);
	valorSpi1b = (int)(valorSPI1);
	UART_send_string("\n\r");
	snprintf(buffer, sizeof(buffer), "P1: %d \r\n", valorSpi1b);
	UART_send_string(buffer);
	_delay_ms(1);
	snprintf(buffer, sizeof(buffer), "P2: %d \r\n", valorSpi2b);
	UART_send_string(buffer);
}

void llegadaADCS(void){
	PORTB &= ~(1<<PORTB2); //Slave Select Bajo
		
		spiWrite('c');
		valorSPI1 = spiRead();
		_delay_ms(10);
		spiWrite('d');
		valorSPI2 = spiRead(); 
		_delay_ms(10);
		
		
		PORTB |= (1<<PORTB2); //Slave Select HIGH
		_delay_ms(250);
}

void IntCont() {

	if (UART_available()) {
		char comando = UART_receive();
		if (comando == 'm') {
			sendUART();
			_delay_ms(100); // Esperar un tiempo para permitir la actualización del ADC
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			IntCont();
			} else if (comando == 'n'){
			//contNUm();
			_delay_ms(100); // Esperar un tiempo para permitir la actualización del ADC
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			IntCont();
			}else {
			UART_send_string("\nEl comando ingresado no se reconoce\n");
		}
	}
}

/*void contNUm(){
	UART_send_string("\n\r");
	// Mensaje inicial para el usuario
	UART_send_string("Por favor, ingresa un número (0-255): ");
	// Lee una cadena de caracteres desde UART
	UART_receive_string(input_buffer, sizeof(input_buffer));

	// Envía un salto de línea antes de imprimir el mensaje recibido
	writeUART('\n');
	writeUART('\r'); // Retorno de carro para asegurar la posición del cursor al inicio de la línea

	UART_send_string("Cadena recibida: ");
	UART_send_string(input_buffer);
	writeUART('\n');
	// Convierte la cadena a un número entero
	received_value = atoi(input_buffer);
	// Envía un salto de línea antes de imprimir el mensaje recibido
	writeUART('\n');
	writeUART('\r'); // Retorno de carro para asegurar la posición del cursor al inicio de la línea
	// Envía un salto de línea antes de imprimir el valor entero
	UART_send_string("Valor recibido como entero: ");
	UART_send_number(received_value);
	// Envía un salto de línea antes de imprimir el mensaje recibido
	writeUART('\n');
	writeUART('\r'); // Retorno de carro para asegurar la posición del cursor al inicio de la línea


	// Verifica si el número está en el rango permitido
	if (received_value >= 0 && received_value <= 255) {
		// Actualiza los LEDs según el valor recibido
		refreshPORT(received_value);
		} else {
		// Maneja valores fuera del rango permitido
		UART_send_string("Valor no válido. Por favor, ingresa un número entre 0 y 255.\n");
	}

	// Solicita al usuario que ingrese otro número
	//UART_send_string("\nPor favor, ingresa un número (0-255): ");
	
}*/
void refreshPORT(uint8_t valor){
	if(valor & 0b10000000){
		PORTD |= (1<<PORTD3);
	}else{
		PORTD &= ~(1<<PORTD3);
	}
	if(valor & 0b01000000){
		PORTD |= (1<<PORTD2);
	}else{
		PORTD &= ~(1<<PORTD2);
	}
	if(valor & 0b00100000){
		PORTC |= (1<<PORTC5);
	}else{
		PORTC &= ~(1<<PORTC5);
	}
	if(valor & 0b00010000){
		PORTC |= (1<<PORTC4);
	}else{
		PORTC &= ~(1<<PORTC4);
	}
	if(valor & 0b00001000){
		PORTC |= (1<<PORTC3);
	}else{
		PORTC &= ~(1<<PORTC3);
	}
	if(valor & 0b00000100){
		PORTC |= (1<<PORTC2);
	}else{
		PORTC &= ~(1<<PORTC2);
	}
	if(valor & 0b00000010){
		PORTC |= (1<<PORTC1);
	}else{
		PORTC &= ~(1<<PORTC1);
	}
	if(valor & 0b00000001){
		PORTC |= (1<<PORTC0);
	}else{
		PORTC &= ~(1<<PORTC0);
	}
	
}

void recibirDatoUART(void){
	if (UCSR0A & (1<<RXC0)) { // Verificar si hay datos recibidos en el buffer
		char dato = UDR0;     // Leer el dato recibido
		if (dato >= '0' && dato <= '9' && indexUART < 3) { // Solo almacenar hasta 3 dígitos
			bufferUART[indexUART++] = dato; // Almacenar el carácter en el buffer
		}
		if (indexUART == 3) { // Si se han recibido 3 caracteres
			valorUART = (bufferUART[0] - '0') * 100 + (bufferUART[1] - '0') * 10 + (bufferUART[2] - '0');
			indexUART = 0; // Reiniciar el índice para el próximo número
		}
	}
}

ISR(USART_RX_vect){
	
	//uint8_t dato = UDR0;  // Leer el dato recibido (para limpiar la bandera de interrupción)
}
