/*
 * CFile1.c
 *
 * Created: 4/15/2021 3:58:57 PM
 *  Author: anuj
 */ 
#define F_CPU 16000000UL
#define BAUD_RATE 38400
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <avr/io.h>		
#include <util/delay.h>		
#include <inttypes.h>		
#include <stdlib.h>		
#include <stdio.h>		
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"

char bt_recv;
char buffer[20];

char buff2[20];

void init() {
	cli();
	
	//motor rotation outputs init to low
	DDRB |= (1<<DDB5);
	DDRB |= (1<<DDB4);
	DDRD |= (1<<DDD3);
	DDRD |= (1<<DDD2);
	PORTD |= (1<<PIND2); 
	PORTD &= ~(1<<PIND3); 
	PORTB |= (1<<PINB5); 
	PORTB &= ~(1<<PINB4); 
	
	//motor enable pins with pwm
	DDRB |= (1 << DDB1)|(1 << DDB2);
	// PB1 and PB2 is now an output

	ICR1 = 0xFFFF;
	// set TOP to 16bit

	OCR1A = 0x3FFF;
	// set PWM for 50% duty cycle @ 16bit

	OCR1B = 0x3FFF;
	// set PWM for 50% duty cycle @ 16bit

	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	// set none-inverting mode

	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// set Fast PWM mode using ICR1 as TOP
	
	TCCR1B |= (1 << CS10);
	// START the timer with no prescaler
	
	sei();
}

int main(void) {
	init();
	UART_init(BAUD_PRESCALER);
	unsigned char temp;
	while(1) {
		temp = UART_receive();
		buffer[0] = (char) temp;
		temp = UART_receive();
		buffer[1] = (char) temp;
		buffer[2] = '\0';
		UART_putstring(buffer);
		
		cli();
		if (strcmp(buffer, "f1") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB |= (1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "25 \n");
			UART_putstring(buff2);
			//25%
			OCR1A = 0x5FFF;
			OCR1B = 0x5FFF;
		} else if (strcmp(buffer, "f2") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB |= (1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "50 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "f3") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB |= (1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "100 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0xFFFF;
			OCR1B = 0xFFFF;
		} else if (strcmp(buffer, "st") == 0) {
			PORTD &= ~(1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB &= ~(1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "STOP \n");
			UART_putstring(buff2);
		} else if (strcmp(buffer, "b1") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB |= (1<<PINB4);
			PORTB &= ~(1<<PINB5);
			sprintf(buff2, "25 \n");
			UART_putstring(buff2);
			//25%
			OCR1A = 0x5FFF;
			OCR1B = 0x5FFF;
		} else if (strcmp(buffer, "b2") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB |= (1<<PINB4);
			PORTB &= ~(1<<PINB5);
			sprintf(buff2, "50 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "b3") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB |= (1<<PINB4);
			PORTB &= ~(1<<PINB5);
			sprintf(buff2, "100 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0xFFFF;
			OCR1B = 0xFFFF;
		} else if (strcmp(buffer, "l1") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB &= ~(1<<PINB5);
			PORTB |= (1<<PINB4);
			sprintf(buff2, "25 \n");
			UART_putstring(buff2);
			//25%
			OCR1A = 0x5FFF;
			OCR1B = 0x5FFF;
		} else if (strcmp(buffer, "l2") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB &= ~(1<<PINB4);
			PORTB &= ~(1<<PINB5);
			sprintf(buff2, "50 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "l3") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB |= (1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "100 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0xFFFF;
		} else if (strcmp(buffer, "r1") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB &= ~(1<<PINB4);
			PORTB |= (1<<PINB5);
			sprintf(buff2, "25 \n");
			UART_putstring(buff2);
			//25%
			OCR1A = 0x5FFF;
			OCR1B = 0x5FFF;
		} else if (strcmp(buffer, "r2") == 0) {
			PORTD &= ~(1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB &= ~(1<<PINB4);
			PORTB |= (1<<PINB5);
			sprintf(buff2, "50 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "r3") == 0) {
			PORTD |= (1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB |= (1<<PINB5);
			PORTB &= ~(1<<PINB4);
			sprintf(buff2, "100 \n");
			UART_putstring(buff2);
			//50%
			OCR1A = 0xFFFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "g2") == 0) {
			PORTD &= ~(1<<PIND2);
			PORTD &= ~(1<<PIND3);
			PORTB &= ~(1<<PINB5);
			PORTB |= (1<<PINB4);
			
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "g3") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB |= (1<<PINB4);
			PORTB &= ~(1<<PINB5);
			 
			//50%
			OCR1A = 0xFFFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "d2") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB &= ~(1<<PINB4);
			PORTB &= ~(1<<PINB5);
			
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0x7FFF;
		} else if (strcmp(buffer, "d3") == 0) {
			PORTD |= (1<<PIND3);
			PORTD &= ~(1<<PIND2);
			PORTB |= (1<<PINB4);
			PORTB &= ~(1<<PINB5);
			
			//50%
			OCR1A = 0x7FFF;
			OCR1B = 0xFFFF;
		}
		sei();
	}
	
}
