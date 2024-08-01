/*
 * SlaveNano.c
 *
 * Created: 25/07/2024 20:44:39
 * Author : luisa
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "SPI/SPI.h"
#include "ADC/ADC.h"

void initTimer0(void);

volatile uint8_t POT1 = 0;
volatile uint8_t POT2 = 0;
volatile uint8_t Turno = 0; 


int main(void)
{
	SetupADC(PC5);
	SetupADC(PC6); 
    spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	initTimer0();
	
	SPCR |= (1<<SPIE); //Activar ISR SPI
	sei(); 
    while (1) 
    {
    }
}

void initTimer0(void){
	TCCR0B |= (1<<CS00)|(1<<CS02);
	TCNT0 = 220;
	TIMSK0 |= (1<<TOIE0);
}

ISR(ADC_vect){
	switch(Turno){
		case 0:
		POT1 = ADCH;
		break;
		case 1:
		POT2 = ADCH;
		break;
		default:
		break;
	}
	ADCSRA |= (1<<ADIF);
}

ISR(TIMER0_OVF_vect){
	switch(Turno){
		case 0:
		Turno = 1;
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)); //PC5
		ADMUX|=(1<<MUX2)|(1<<MUX0);
		break;
		case 1:
		Turno = 0;
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX2)|(1<<MUX1);
		break;
	}
	ADCSRA |= (1<<ADSC);
	//TCNT0 = 250;
	TCNT0 = 220;
	TIFR0 |= (1 << TOV0);
}

ISR(SPI_STC_vect){
	uint8_t spiValor = SPDR; 
	if(spiValor == 'c'){
		spiWrite(POT1);
		//
	}
	if(spiValor == 'd'){
		spiWrite(POT2);
	}
}