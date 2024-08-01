/*
 * SPI.c
 *
 * Created: 25/07/2024 19:45:00
 *  Author: luisa
 */ 
#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase)
{
	//PB2 -> SS
	//PB3 -> MOSI
	//PB4 -> MISO
	//PB5 -> SCK Reloj
	if(sType & 0b00010000) //If Master Mode
	{
		DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB5); //SS, MOSI, SCK
		DDRB &= ~(1<<DDB4); //MISO
		SPCR = (1<<MSTR); //Master
		uint8_t temp = sType & 0b00000111; 
		switch(temp){
			case 0: //DIV2
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR |= (1<<SPI2X);
			break;
			case 1: //DIV4
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR &= ~(1<<SPI2X);
			break;
			case 2: //DIV8
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1); 
				SPSR |= (1<<SPI2X);
			break;
			case 3: //DIV16
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 4: //DIV32
				SPCR |= (1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 5: //DIV64
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 6: //DIV128
				SPCR |= (1<<SPR1)|(1<<SPR0);
				SPSR &= ~(1<<SPI2X);
			break; 
		}
	}
	else //If Slave
	{
		DDRB |= (1<<DDB4); //MISO
		DDRB &= ~((1<<DDB3)|(1<<DDB5)|(1<<DDB2)); //MOSI, SCK, SS
		SPCR &= ~(1<<MSTR); //Slave
	}
	/*Enable SPI, Data Order, Clock Polarity, Clock Phase*/
	SPCR |= (1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
	
}

void spiWrite(uint8_t dat){ //Escribe datos al bus para SPI
	SPDR = dat;
}

unsigned spiDataReady(){ //Chequea si la información del Buzzer esta lista para leerse 
	if(SPSR & (1<<SPIF))
	return 1;
	else
	return 0; 
}

uint8_t spiRead(void)
{
	while (!(SPSR & (1<<SPIF))); //Espera para recibir los datos con la bandera encendida 
	return(SPDR); //Lee la data del buffer
}