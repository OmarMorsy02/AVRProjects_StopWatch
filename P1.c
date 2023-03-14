/*
 * P1.c
 *
 *  Created on: Feb 28, 2023
 *      Author: amor2
 */

#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>

unsigned char START = 0;
unsigned char sec1 = 0;
unsigned char sec2 = 0;
unsigned char min1 = 0;
unsigned char min2 = 0;
unsigned char counter = 0;

//Seconds Timer interrupt
ISR(TIMER0_OVF_vect) {
	counter++;
	if (counter == 4) {
		counter = 0;
		sec1++;
		if (sec1 > 9) {
			sec1 = 0;
			sec2++;
		}

		if (sec2 > 5) {
			min1++;
			sec2 = 0;
		}

		if (min1 > 9) {
			min2++;
			min1 = 0;
		}
	}
}
//THE RESET button interrupt
ISR(INT0_vect)
{
	sec1=sec2=min1=min2=0;
	START = 0;
}


void main() {
	DDRC |= 0x0F;
	DDRA |= 0x0F;
	DDRB |=(1<<PB0);
	DDRB |=(1<<PB1);
	PORTC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	TCCR0 |= (1 << FOC0) | (1 << CS00) | (1 << CS02);
	SREG |= (1 << 7);
	TIMSK |= (1 << TOIE0);
	MCUCR |= (1<<ISC01) | (1<<ISC00);
	GICR |= (1<<INT0);
	while (1) {
		while(START==0)
		{
			sec1=sec2=min1=min2=0;
			if(PINB &(1<<PB0))
				START = 1;
		}
		if(PINB & (1<<PB1))
		{
			TIMSK &= ~(1 << TOIE0);
			while(1)
			{
				if(PINB & (1<<PB0))
					{
					TIMSK |= (1 << TOIE0);
					break;
					}
			}
		}
		if (min2 > 9) {
			sec2 = 0;
			min1 = 0;
			min2 = 0;
		}
		PORTA &= 0x0;
		PORTA |= (1 << PA0);
		PORTC = (PORTC & 0xF0) | (sec1 & 0x0F);
		_delay_ms(50);
		if (sec2 >= 0) {
			PORTA &= 0x0;
			PORTA |= (1 << PA1);
			PORTC = (PORTC & 0xF0) | (sec2 & 0x0F);
			_delay_ms(50);
		}
		if (min1 >= 1) {

			PORTA &= 0x0;
			PORTA |= (1 << PA2);
			PORTC = (PORTC & 0xF0) | (min1 & 0x0F);
			_delay_ms(50);
		}
		if (min2 >= 1) {
			PORTA &= 0x0;
			PORTA |= (1 << PA3);
			PORTC = (PORTC & 0xF0) | (min2 & 0x0F);
			_delay_ms(50);
		}
	}
}
