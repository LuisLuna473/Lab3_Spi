/*
 * ADC.c
 *
 * Created: 25/07/2024 20:47:32
 *  Author: luisa
 */ 
#include "ADC.h"
void SetupADC(uint8_t PinADC){
	switch (PinADC)
	{
		case PC0 :
		DIDR0 |= (1<<ADC0D);
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		break;
		case PC1 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX0);
		DIDR0 |= (1<<ADC1D);
		break;
		case PC2 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX1);
		DIDR0 |= (1<<ADC2D);
		break;
		case PC3 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX0)|(1<<MUX1);
		DIDR0 |= (1<<ADC3D);
		break;
		case PC4 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX2);
		DIDR0 |= (1<<ADC4D);
		break;
		case PC5 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX2)|(1<<MUX0);
		DIDR0 |= (1<<ADC5D);
		break;
		case PC6 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX2)|(1<<MUX1);
		break;
		case PC7 :
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX|=(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
		break;
	}
	//JUSTIFICACION IZQUIERDA
	ADMUX |= (1<<ADLAR);
	//REFERENCIA INTERNA
	ADMUX |= (1<<REFS0);
	// HABILITAR INTERRUPCION
	ADCSRA |= (1<<ADIE);
	//PRESCALER 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//HABILITAR
	ADCSRA |= (1<<ADEN);
}