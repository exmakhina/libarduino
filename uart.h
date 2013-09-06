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

#ifndef _UART_H_
#define _UART_H_

#include <stdio.h>
#include "libarduino.h"

#ifdef ENABLE_SERIAL
#define UART_BAUD_RATE			9600 /* default is 9600 */ 
#define UART_BAUD_SELECT		(F_CPU / (UART_BAUD_RATE * 16L) - 1)
#define UART_BUFFER_SIZE	16
//#define ECHO_MODE_ENABLE

#if	defined(__AVR_ATmega168__) || \
 	defined(__AVR_ATmega328P__)

#define UART0_DATA	UDR0
#define UART0_ISR_VECT USART_RX_vect

#else
// unsupported type
#error "Processor type not supported in uart.h !"
#endif

/* to use serial functions, do init then use printf() and getchar() */
void serial_init(void);
int serial_getchar(FILE* stream);
int serial_putchar(char data, FILE* stream);
#endif	// ENABLE_SERIAL

#endif	// _UART_H_
