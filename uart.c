/* ***********************************************************************
**  libarduino - a GNU library for using GNU toolchain with Arduino
**  Copyright (C) 2009 Michael Spiceland
*************************************************************************
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software Foundation, 
**  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
*************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libarduino.h"

#ifdef ENABLE_SERIAL
uint8_t uart_buffer[UART_BUFFER_SIZE];
volatile uint8_t uart_readptr;
volatile uint8_t uart_writeptr;

ISR(UART0_ISR_VECT)
{
	uart_buffer[uart_writeptr] = UART0_DATA;
#ifdef ECHO_MODE_ENABLE
	while ( !(UCSR0A & (1<<UDRE0)) ); /* block till we have space */
	UART0_DATA = uart_buffer[uart_writeptr]; /* send */
#endif
	uart_writeptr = (uart_writeptr + 1) % UART_BUFFER_SIZE;
}

void serial_init(void)
{
	uart_writeptr = 0;
	uart_readptr = 0;

	// set default baud rate 
	UBRR0H = UART_BAUD_SELECT >> 8;  
	UBRR0L = UART_BAUD_SELECT;

	// enable receive, transmit and ensable receive interrupts 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);

	/* dont forget sei() */
}

int serial_getchar(FILE* stream)
{
	uint8_t c;

	while( uart_writeptr == uart_readptr ); /* block waiting */
	c = uart_buffer[uart_readptr];    	
	uart_readptr = (uart_readptr + 1) % UART_BUFFER_SIZE;
  
	return c;
}

int serial_putchar(char data, FILE* stream)
{   
	while ( !(UCSR0A & (1<<UDRE0)) ); /* block till we have space */
	UART0_DATA = data; /* send */
	return data;
}
#endif
